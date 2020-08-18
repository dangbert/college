import numpy as np
import cv2
from sklearn.cluster import KMeans
from scipy import spatial
import math
from PIL import Image
from normalize import normalize_features

# compute features and normalize (if needed)
# returns a list of features (in same order as imdb.image_names)
def compute_features(imdb, args, useValSet=False):
    if args.feature == 'tinyimage':
        features = tinyimage_features(imdb, args.tinyimage_patchdim)
    elif args.feature == 'bow-patches':
        features = bow_patch_features(
            imdb, args.patches_dictionarysize, args.patches_radius,
            args.patches_stride, useValSet=useValSet)
    elif args.feature == 'bow-sift':
        features = bow_sift_features(
            imdb, args.sift_dictionarysize, args.sift_radius,
            args.sift_stride, args.sift_binsize, useValSet=useValSet)
    else:
        raise NotImplementedError('Selected feature not yet implemented')

    if args.feature != 'tinyimage':
        print('normalizing')
        features = normalize_features(features)
    return features

"""
Simply resizes the images to a fixed [n n 3] and makes it a vector of size [1 nxnx3].
Performed on all images because main will handle selecting the desired images' features for training
Returns an np array where each row is a feature vector for an image
"""
def tinyimage_features(imdb, patchdim):
    features = []
    for fname in imdb.image_names:
        img = cv2.imread(imdb.image_dir + "/" + fname)
        img = cv2.resize(img, (patchdim, patchdim)) 
        img = img.flatten()
        features.append(list(img))
    return np.array(features)

"""
Take a subset of images in the train + val subset and extract several thousand features from them
useValSet: whether or not to utilitze the validation set when creating the dictionary
"""
def bow_patch_features(imdb, dictionarysize, radius, stride, useValSet=False):
    if useValSet:
        print("using val set also")
        imgSet = np.concatenate((imdb.train_indices, imdb.val_indices), axis=0).copy()
    else:
        print("using just the train set:")
        imgSet = imdb.train_indices.copy()
    print(str(len(imgSet)) + " images in set")
    np.random.shuffle(imgSet)

    #### build a dictionary of words:
    # iterate over all images in imgSet and extract patches
    patches = []
    for i in imgSet:
        patches += getPatches(i, imdb, dictionarysize, radius, stride)
    np.random.shuffle(patches)                  # shuffle patches
    if len(patches) > 110 * dictionarysize:     # only use some patches to speed up time of kmeans
        patches = patches[:110 * dictionarysize]
    print("gathered " + str(len(patches)) + " patches from " + str(len(imgSet)) + " images")

    #### create our dictionary ("bag of words")
    kmeans = KMeans(n_clusters=dictionarysize).fit(np.array(patches))
    dictionary = kmeans.cluster_centers_
    myTile(dictionary, radius)                  # save visualization of dictionary
    print("created dictionary")

    #for c in dictionary[:50]:   # visualize some of the centers:
    #    showImage(c.reshape((2*radius+1, 2*radius+1)).astype('uint8'))

    #### now get the features for each image:
    def patchToWord(patch): # for bow_patch, the patches themselves are words
        return patch
    return getFeatures(kmeans, imdb, dictionarysize, radius, stride, flatten=True, patchToWord=patchToWord)

"""
compute the SIFT feature for the center pixel inside each patch
useValSet: whether or not to utilitze the validation set when creating the dictionary
"""
# https://ianlondon.github.io/blog/how-to-sift-opencv/
# TODO: use the binsize
def bow_sift_features(imdb, dictionarysize, radius, stride, binsize, useValSet=False):
    if useValSet:
        imgSet = np.concatenate((imdb.train_indices, imdb.val_indices), axis=0).copy()
    else:
        imgSet = imdb.train_indices.copy()
    sift = cv2.xfeatures2d.SIFT_create()        # had to do pip3 install opencv-contrib-python (for SIFT)

    # shuffled set of indices we can take a random sample from
    np.random.shuffle(imgSet)

    # convert a patch of an image to a (sift) descriptor
    def patchToWord(patch):
        # https://www.cc.gatech.edu/~afb/classes/CS4495-Fall2014/ProblemSets/PS4/ps4-supplemental.pdf#page=3
        dim = int(patch.shape[0]/2)
        # (this won't be angle invarient)
        point = cv2.KeyPoint(x=dim, y=dim, _size=binsize, _angle=0, _octave=0)
        kp, des = sift.compute(patch,[point])   # calculate descriptor at this point
        return des[0]

    #### build a dictionary of features:
    # iterate over random subset of images and extract features
    patches = []
    for i in imgSet:
        patches += getPatches(i, imdb, dictionarysize, radius, stride, flatten=False)
    np.random.shuffle(patches)                  # shuffle
    if len(patches) > 150 * dictionarysize:     # only use some to speed up time of kmeans
        patches = patches[:150 * dictionarysize]
    descriptors = [patchToWord(patch) for patch in patches]  # convert patches to descriptors

    print("gathered " + str(len(descriptors)) + " descriptors")
    #### create our dictionary ("bag of words")
    kmeans = KMeans(n_clusters=dictionarysize).fit(np.array(descriptors))
    dictionary = kmeans.cluster_centers_
    print("created dictionary")

    #### now get the features for each image:
    return getFeatures(kmeans, imdb, dictionarysize, radius, stride, flatten=False, patchToWord=patchToWord)

# get a feature vector for each image in imdb
# (a feature is a histogram of the elements in the dictionary that the image contains)
# flatten: whether each patch should be flattened into a vector
# patchToWord: (function that converts a patch to a "word" to match against in the dictionary)
# returns a list of features (in same order as imdb.image_names)
def getFeatures(kmeans, imdb, dictionarysize, radius, stride, flatten, patchToWord):
    features = []
    #dictionary = kmeans.cluster_centers_
    # for easily computing euclidean distances https://stackoverflow.com/a/32446753
    print("getting features in each image")
    count = 0
    for i in range(len(imdb.image_names)):
        feat = [0] * dictionarysize             # feature for this image
        patches = getPatches(i, imdb, dictionarysize, radius, stride, flatten)
        if count % 50 == 0:
            print(str(len(patches)) + " patches in image " + str(i))
        count += 1
        words = [patchToWord(patch) for patch in patches]
        # (for each patch, find the most similar dictionary element)
        results = kmeans.predict(words)         # this is MUCH faster than doing 1 patch at a time
        for index in results:
            feat[index] += 1                    # create historgram of features
        features.append(feat)
    return np.array(features)

# get a list of patches (square sections) in a given image
# get an array of patches (sections of given size and spacing) in an image
# i: index of the image in imdb.image_names
# flatten: whether each patch should be flattened into a vector
def getPatches(i, imdb, dictionarysize, radius, stride, flatten=True):
    patches = []
    gray = cv2.imread(imdb.image_dir + "/" + imdb.image_names[i], cv2.IMREAD_GRAYSCALE)
    #label = imdb.class_ids[i]
    # extract features from this image:
    for row in range(radius, gray.shape[0] - 1 - radius, stride):
        for col in range(radius, gray.shape[1] - 1 - radius, stride):
            patch = gray[(row-radius):(row+radius+1), (col-radius):(col+radius+1)]
            if flatten:
                patches.append(patch.flatten())
            else:
                patches.append(patch)
    #print("image " + str(i) + " has " + str(len(patches)) + " patches")
    return patches

def showImage(img):
    cv2.imshow("image", img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


# place images of the same (square) size into a single image (tiled grid)
# based on: https://stackoverflow.com/a/30228308
# d: list of images of identical sizes
# radius: radius used to create the patches in the dictionary
def myTile(d, radius):
    cols = int(math.sqrt(len(d)))               # number of images per row
    rows = cols
    while (rows * cols) < len(d):
        rows += 1
    combined = np.zeros((rows*(2*radius+1), cols*(2*radius+1)), dtype=np.uint8) + 255
    comb = Image.fromarray(combined , 'L')  # conver to PIL image
    i = -1 
    for yoff in range(rows):
        for xoff in range(cols):
            i += 1
            if i >= len(d):
                break
            patch = d[i].reshape((2*radius+1, 2*radius+1)).astype('uint8')
            img = Image.fromarray(patch , 'L')
            comb.paste(img, (xoff*(2*radius+1), yoff*(2*radius+1)))
    #comb.show()
    comb.save("dictionary.jpg")
    print("created tile")
