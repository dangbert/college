# Import the functions to calculate feature descriptors
from skimage.feature import local_binary_pattern
from skimage.feature import hog
from skimage.io import imread
from sklearn.externals import joblib
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
DATA_DIR = "/home/dan/projects/cmsc491/Bosch_Traffic_Dataset"
OUT_PATH = "../myData" # directory to save image patches into
Rectangle = namedtuple('Rectangle', 'ymin ymax xmin xmax')

def main():
    fileCount = 0    # number of files saved (for filename)
    trafficCount = 0 # number of traffic lights saved
    otherCount = 0   # number of non traffic light patches saved
    skipCount = 0    # number of traffic lights skipped
    if not createFolders():
        print("please delete the existing image folders first")
        return

    #testData = get_all_labels(DATA_DIR + "/rgb_test/test.yaml")
    trainData = get_all_labels(DATA_DIR + "/rgb_train/train.yaml")
    print("trainData length: " + str(len(trainData)))
    # shuffle and sample first n images in training
    indices = [i for i in range(len(trainData))]
    random.shuffle(indices)
    #indices = indices[:1000] # TODO: number of images to sample

    #fds = []  # all features (need to include ones that are not traffic lights as well)
    # extract patches from images and save them to labeled folders
    for i in indices:
        info = trainData[i]
        image = cv2.imread(DATA_DIR + "/rgb_train/train/" + info['path'], cv2.IMREAD_GRAYSCALE)
        if fileCount % 1500 == 0:
            print("file count: " + str(fileCount))
        # sample some places in this image that aren't traffic lights
        for _ in range(30): # number of random samples per image
            rect = getRandRect(image)
            if isALight(rect, info['boxes']):   # make sure it's not a traffic light
                continue
                #cv2.rectangle(imageCopy, (int(rect.xmin),int(rect.ymin)+1), (int(rect.xmax),int(rect.ymax)+1), [255,255,255], 2)
            img = getRegion(image, rect)
            saveImage(img, None, fileCount)
            otherCount += 1
            fileCount += 1

        for box in info['boxes']:
            rect = Rectangle(box['y_min'], box['y_max'], box['x_min'], box['x_max'])
            if not validCord(rect, image):
                #print("skipping image with shape: " + str(int(box['y_max']-box['y_min'])) + "x" + str(int(box['x_max']-box['x_min'])) )
                skipCount += 1
                continue

            # get 24x48 image patch corresponding to this box
            img = getRegion(image, rect)
            saveImage(img, box, fileCount)
            trafficCount += 1
            fileCount += 1

    print("***Completed calculating features from training images***")
    print("lights skipped: " + str(skipCount))
    print("lights saved: " + str(skipCount))
    print("others saved: " + str(otherCount))
    print("TOTAL patches saved: " + str(fileCount))

# get a random Rectangle for an image
# returns a Rectangle
def getRandRect(image):
    while True:
        y = random.randint(0, image.shape[0]-1)
        x = random.randint(0, image.shape[1]-1)
        r = random.randint(0,4)
        if r == 0:
            width = random.randint(40, 75)
        if r == 1:
            width = random.randint(15, 40)
        else:
            width = random.randint(8, 30)   # most lights are smaller
        height = int(random.uniform(1.0, 2.8) * width)
        rect = Rectangle(y, y+height, x, x+width)
        if validCord(rect, image):
            return rect

# return True if given coordinates are good to use for extracting a patch
# (some were negative if the light was not fully in the image)
# r: Rectangle
def validCord(r, image):
    if r.xmax >= image.shape[1] or r.ymax >= image.shape[0]:
        return False
    # TODO: do we want to exclude small images???
    if not (r.ymax-r.ymin > 16 and r.xmax-r.xmin > 8):
        return False
    if not (r.ymin >= 0 and r.ymax >= 0 and r.xmin >= 0 and r.xmax >= 0):
        return False
    return True

# determine if a given rectangle is a traffic light in a given image
# rect: Rectangle
# boxes: list of boxes in the image
def isALight(rect, boxes):
    for box in boxes:
        rect2 = Rectangle(box['y_min'], box['y_max'], box['x_min'], box['x_max'])
        area = intersectArea(rect, rect2)
        if area != None:
            return True
    return False

# return the intersection area between two rectangles
# returns None if rectangles don't intersect
# useful for sampling portions of images that aren't traffic lights
# also useful for testing (evaluating if it got it right)
# code from: https://stackoverflow.com/a/27162334
def intersectArea(a, b):
    dx = min(a.xmax, b.xmax) - max(a.xmin, b.xmin)
    dy = min(a.ymax, b.ymax) - max(a.ymin, b.ymin)
    if (dx>=0) and (dy>=0):
        return dx*dy

# crop image as desired and resize it to be: 24x48
# expand: whether or not to expand the image outside its border by 20% while resizing
# image: full image
# rect: Rectangle where the desired patch is
def getRegion(image, rect, expand=True):
    ymin, ymax, xmin, xmax = rect.ymin, rect.ymax, rect.xmin, rect.xmax
    # grow the region to make the ratio 2:1
    img = image[int(ymin):int(ymax)+1, int(xmin):int(xmax)+1]
    ratio = (ymax-ymin+1) / (xmax-xmin+1) 
    if ratio > 2:    # expand horizontally
        goal = (ymax-ymin+1) / 2 # what we want the width to be
        add = (goal - (xmax-xmin+1)) / 2
        xmin -= add if (xmin-add) >= 0 else 0
        xmax += add if (xmax+add) < image.shape[1] else 0
    elif ratio < 2:  # expand vertically
        goal = (xmax-xmin+1) * 2 # what we want the height to be
        add = (goal - (ymax-ymin+1)) / 2
        ymin -= add if (ymin-add) >= 0 else 0
        ymax += add if (ymax+add) < image.shape[0] else 0

    # now expand borders by another 20% (because they're really tight around the lights)
    if expand:
        xRange, yRange = (xmax-xmin)*1.2, (ymax-ymin)*1.2     # goal dimensions
        xMid, yMid = xmin+(xmax-xmin)/2, ymin+(ymax-ymin)/2   # midpoints
        xmin = (xMid-xRange/2) if (xMid-xRange/2) >= 0 else xmin
        xmax = (xMid+xRange/2) if (xMid+xRange/2) < image.shape[1] else xmax
        ymin = (yMid-yRange/2) if (yMid-yRange/2) >= 0 else ymin
        ymax = (yMid+yRange/2) if (yMid+yRange/2) < image.shape[0] else ymax

    #print("cords: " + str(ymin) + "-" + str(ymax) + "   " + str(xmin) + "-" + str(xmax))
    #print("ratio is " + str((ymax-ymin+1)/(xmax-xmin+1)))
    img = image[int(ymin):int(ymax)+1, int(xmin):int(xmax)+1] # crop image
    # now resize image to desired size
    img = cv2.resize(img, (24,48))
    return img

# save a given image to the correct folder
# converts all classes to be either:
#  other (not a traffic light)
#  red
#  yellow
#  green
#  off
def saveImage(img, box, fname):
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
    else:
        folder = "other"
    path = OUT_PATH + "/" + folder + "/" + str(fname) + ".jpg"
    im = Image.fromarray(img)
    im.save(path)

# create all the folders to store the images
# return false if one or more of the folders already existed
def createFolders():
    for folder in ["other", "red", "yellow", "green", "off"]:
        path = OUT_PATH + "/" + folder
        if not os.path.exists(path):
            os.makedirs(path)
        else:
            return False
    return True

if __name__ == "__main__":
    main()
