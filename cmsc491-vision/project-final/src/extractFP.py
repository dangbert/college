# this file extracts false positives and saves them to the "other" folder
# this file is a combination of testClassifier.py and extractFeatures.py
# TODO: clean code if I have time
from skimage.feature import local_binary_pattern
from skimage.feature import hog
from skimage.io import imread
from sklearn.externals import joblib
from skimage.transform import pyramid_gaussian
# To read file names
import argparse as ap
import glob
import os
from config import *
import numpy as np
import cv2
import random
from PIL import Image
from collections import namedtuple
import matplotlib.pyplot as plt
from skimage.feature import hog
from read_label_file import get_all_labels   # modified version of script from BOSCH traffic dataset github
from testClassifier import sliding_window    # use this to get features in my images
from extractPatches import getRegion, isALight
DATA_DIR = "/home/dan/projects/cmsc491/Bosch_Traffic_Dataset"
OUT_PATH = "../myData" # directory to save image patches into
Rectangle = namedtuple('Rectangle', 'ymin ymax xmin xmax')

def main():
    createFolder()
    trainData = get_all_labels(DATA_DIR + "/rgb_train/train.yaml")
    print("trainData length: " + str(len(trainData)))
    # don't shuffle for this (so we can easily run multiple programs at the same time)
    indices = [i for i in range(len(trainData))]
    #indices = indices[1500:2000] # did this for each process in batches of 500
    random.shuffle(indices)
    print("LOOKING FOR FALSE POSITIVES")
    #indices = indices[:1000] # TODO: number of images to sample
    fileCount = 0
    imageCount = 0

    #####################################
    # Read the image
    min_wdw_sz = (24, 48)
    step_size = (5, 5)
    downscale = 1.25
    # Load the classifier
    # TODO: update to desired model
    # model4 is from run2 (all images, and 15ish "other" per image, but train was limited to 20,000 from "other")
    clf = joblib.load("../myData/traffic.model5")
    #####################################

    #fds = []  # all features (need to include ones that are not traffic lights as well)
    # extract patches from images and save them to labeled folders
    for i in indices:
        i = 137
        curCount = 0 # number of patches from this image so far
        loc = i
        imageCount += 1
        info = trainData[i]
        im = cv2.imread(DATA_DIR + "/rgb_train/train/" + info['path'], cv2.IMREAD_GRAYSCALE)
        if fileCount % 100 == 0:
            print("patches saved so far: " + str(fileCount))
            print("images so far: " + str(imageCount))

        imageCopy = cv2.cvtColor(im.copy(),cv2.COLOR_GRAY2RGB) # copy to draw rectangles on
        #####################################
        for im_scaled in pyramid_gaussian(im, downscale=downscale):
            # This list contains detections at the current scale
            cd = []
            # If the width or height of the scaled image is less than
            # the width or height of the window, then end the iterations.
            if im_scaled.shape[0] < min_wdw_sz[1] or im_scaled.shape[1] < min_wdw_sz[0]:
                break
            for (x, y, im_window) in sliding_window(im_scaled, min_wdw_sz, step_size):
                if im_window.shape[0] != min_wdw_sz[1] or im_window.shape[1] != min_wdw_sz[0]:
                    continue
                # Calculate the HOG features
                # TODO!!!: is it a problem that im_window is grayscale floats?  will that mess with my SVM which was trained with grayscale images from 0-255?
                fd = hog(im_window, orientations, pixels_per_cell, cells_per_block, block_norm="L1", visualise=False, transform_sqrt=True, feature_vector=True)
                fd = [fd] # clf wants a list of samples
                # TODO: it's faster to pass all the samples to clf at once to predict on
                #pred = clf.predict(fd)
                probs = list(clf.predict_proba(fd)[0])
                #print(pred)
                pred = probs.index(max(probs)) # index of max
                if pred != 0 and probs[pred] > 0.95:
                    # check if this is a false positive (need to find its rect)
                    # calculate (approximate) rect for this region (24x48)
                    scaleF = im.shape[0] / im_scaled.shape[0]
                    scaleF2 = im.shape[1] / im_scaled.shape[1]
                    if scaleF2 > scaleF: # take the bigger one
                        scaleF = scaleF2
                    # TODO: fix problem where some correct traffic lights are ending up in the falsePositives (I'm pretty sure of this but it's a little hard to tell with some of them)
                    rect = Rectangle(y, y+int(48/scaleF), x, x+int(25/scaleF))
                    print("\n" + str(loc) + ": " + str(rect))
                    #test = getRegion(im, rect)
                    # TODO: does this function not work??? (see above problem TODO)
                    if not isALight(rect, info['boxes']):
                        isLight = "NOT LIGHT"
                        cv2.rectangle(imageCopy, (int(rect.xmin),int(rect.ymin)+1), (int(rect.xmax),int(rect.ymax)+1), [255,0,0], 2)
                        saveImage(im_window, None, "fp-" + str(loc) + "-" + str(curCount), "falsePositives", convert=True)
                        # im_window is float (values from 0 to 1) so convert:
                    else:
                        isLight = "LIGHT"
                        cv2.rectangle(imageCopy, (int(rect.xmin),int(rect.ymin)+1), (int(rect.xmax),int(rect.ymax)+1), [0,255,0], 2)
                        # store positive matches as well for reference:
                        saveImage(im_window, None, "p-" + str(loc) + "-" + str(curCount), "positives", convert=True)
                    fileCount += 1
                    curCount += 1
                    # TODO: there is a definite bug here where some patches are incorrectly believed to
                    # be a light or not a light (might be a problem with how rect is created)
                    # once instance of the problem is apprent in image 137, patch 11
                    saveImage(imageCopy, None, "typ-summary-" + str(loc) + "summaries", "summaries")
                    if curCount == 11:
                        print("" + str(curCount) + ": " + str(rect))
                        plt.imshow(im_window)
                        plt.title(isLight)
                        plt.show()
                        copy2 = cv2.cvtColor(im.copy(),cv2.COLOR_GRAY2RGB) # copy to draw rectangles on
                        cv2.rectangle(copy2, (int(rect.xmin),int(rect.ymin)+1), (int(rect.xmax),int(rect.ymax)+1), [255,0,0], 2)
                        plt.imshow(copy2)
                        plt.title(isLight)
                        plt.show()
            #####################################
        saveImage(imageCopy, None, "summary-" + str(loc) + "summaries", "summaries")
        return

    print("*** Completed looking for false positiives ***")
    print("TOTAL patches saved: " + str(fileCount))

# save a given image to the correct folder
# converts all classes to be either:
#  other (not a traffic light)
#  red
#  yellow
#  green
#  off
def saveImage(img, box, fname, folder="", convert=False):
    if folder == "":
        folder = "other"
        if box != None:
            if "Red" in box['label']: # this way we treat RedRight as red, etc.
                folder = "red"
            if "Green" in box['label']:
                folder = "green"
            if "Yellow" in box['label']:
                folder = "yellow"
            if "off" == box['label']:
                folder = "off"
    path = OUT_PATH + "/" + folder + "/" + str(fname) + ".jpg"
    if convert:
        # https://stackoverflow.com/a/26918508
        img = (img * 255).round().astype(np.uint8)
        #img = Image.fromarray(img.astype(np.uint8), mode='L')
    img = Image.fromarray(img)
    img.save(path)

# create all the folders to store the images
# return false if one or more of the folders already existed
def createFolder():
    for folder in ["falsePositives", "positives", "summaries"]:
        path = OUT_PATH + "/" + folder
        if not os.path.exists(path):
            os.makedirs(path)

if __name__ == "__main__":
    main()
