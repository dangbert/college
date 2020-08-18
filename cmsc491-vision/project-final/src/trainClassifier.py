# Import the required modules
from skimage.feature import local_binary_pattern
from sklearn.svm import LinearSVC, SVC
from sklearn.linear_model import LogisticRegression
from sklearn.externals import joblib
import argparse as ap
import glob
import os
from config import *
import numpy as np
import random
import matplotlib.pyplot as plt
from skimage.feature import hog
from read_label_file import get_all_labels   # modified version of script from BOSCH traffic dataset github
from testClassifier import *   # use this to get features in my images
DATA_DIR = "/home/dan/projects/cmsc491/Bosch_Traffic_Dataset"
OUT_PATH = "../myData" # directory containing image patches

if __name__ == "__main__":
    # Parse the command line arguments
    parser = ap.ArgumentParser()
    parser.add_argument('-c', "--classifier", help="Classifier to be used", default="LIN_SVM")
    args = vars(parser.parse_args())

    # Classifiers supported
    clf_type = args['classifier']
    mapping = {
            "other": 0,
            "red": 1,
            "yellow": 2,
            "green": 3,
            "off": 4
    }

    # patches statistics:
    #     lights skipped: 5258
    #     lights saved: 5258
    #     others saved: 151919
    #     TOTAL patches saved: 157417

    # read in the image patches and get their features
    # TODO: consider shuffling first
    fds = []
    labels = []
    counts = [0, 0, 0, 0, 0]     # number of images used so far for each
    limits = [0, 0, 0, 0, 0]     # number of images to use from each (0 for no limit)
    folders = ["other", "red", "yellow", "green", "off"]
    for folder, limit in zip(folders, limits):
        path = OUT_PATH + "/" + folder
        ###path = "../myData/other-not-using/"
        # stop after we've read enough samples
        for feat_path in glob.glob(os.path.join(path, "*.jpg")):
            """
            # move files around to make other have less or more random pics
            if folder == "other" and random.randint(1, 100) <= 14:  # keep 1/5 files
                print(feat_path)
                s = feat_path
                s = "../myData/other/" + s[s.find("/other") + 17:]
                print(s)
                #os.rename(s, feat_path) # move file
                os.rename(feat_path, s) # move file
                """

            if limits[mapping[folder]] != 0 and counts[mapping[folder]] >= limits[mapping[folder]]:
                break
            img = cv2.imread(feat_path, cv2.IMREAD_GRAYSCALE) # read img from file
            # get HOG feature for this image (all images must be same size)
            fd = hog(img, orientations, pixels_per_cell, cells_per_block, block_norm="L1", visualise=False, transform_sqrt=True, feature_vector=True)
            fds.append(fd)
            labels.append(mapping[folder])
            counts[mapping[folder]] += 1
    print("length of features vector: " + str(len(fds)))

    # train the classifier
    #clf = LinearSVC()
    clf = SVC(kernel='linear',probability=True)
    print("***Training a Linear SVM Classifier***")
    clf.fit(fds, labels)
    print("***done training***")
    # If feature directories don't exist, create them
    if not os.path.isdir(os.path.split(model_path)[0]):
        os.makedirs(os.path.split(model_path)[0])
    model_path = OUT_PATH + "/traffic.model"
    joblib.dump(clf, model_path)
    print("Classifier saved to {}".format(model_path))
