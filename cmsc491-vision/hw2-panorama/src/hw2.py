#!/usr/bin/env python3
# Dan Engbert
# Computer Vision - Spring 2018
# This program combines two images into a panorma
#
# The relevant cv2 functions are:
# goodFeaturesToTrack(), findHomography(), getPerspectiveTransform and warpPerspective
# for this program I make use of cv2.goodFeaturesToTrack() and cv2.warpPerspective()
# process the points myself, and then implement the other two functions myself
import numpy as np
from PIL import Image                           # (pip3 install pillow)
import cv2                                      # (pip3 install opencv-python)
import sys
from matplotlib import pyplot as plt
import random

def main():
    # tower panorma:
    print("creating tower panorama:")
    result = createPano(cv2.imread('data/tower/1.JPG'), cv2.imread('data/tower/2.JPG'))
    showImage(result)
    cv2.imwrite("pano-tower.jpg", result)

    # create panoramas for each of the sets with 3 images
    for fname in ["hill", "ledge", "pier"]:     # folder names
        print("\ncreating " + fname + " panorama:")
        img1 = cv2.imread('data/' + fname + '/1.JPG')
        img2 = cv2.imread('data/' + fname + '/2.JPG')
        result = createPano(cv2.imread('data/' + fname + '/1.JPG'), cv2.imread('data/' + fname + '/2.JPG'))
        result = createPano(result, cv2.imread('data/' + fname + '/3.JPG'))
        showImage(result)
        cv2.imwrite('pano-' + fname + '.jpg', result)


# returns a panorma of img1 and img2
def createPano(img1, img2):
    pairs = getDescriptorPairs(img1, img2)      # get the best descriptor pairs to use

    H = ransac(img1, img2, pairs)               # run RANSAC
    # TODO: undestand why I had to pass img2 first to get this to work (backwards)
    result = applyHomography(H, img2, img1)     # apply the perspective transform
    return result

    # testing pairs using findHomography() (to see how useful the points are for finding homography)
    #print('\ntesting on pairs')
    #dst_pts = []
    #src_pts = []
    #for pair in pairs:                          # populate lists with points in (x,y) format
    #    dst_pts.append(list(pair[0]))
    #    src_pts.append(list(pair[1]))
    #H, _ = cv2.findHomography(np.float32(src_pts), np.float32(dst_pts), cv2.RANSAC, 5.0) # returns the same model if findHomography was only given 4 points
    #print(H)
    #showImage(applyHomography(H, img1, img2))


# create a model using a given subset of pairs
# subset: list of 4 indicies within pairs to use for training
# return tuple: (model, inlinerCount)
def createModel(pairs, subset, epsilon):
    train1 = []               # points in img1 for creating model
    train2 = []               # points in img2 for creating model
    test1  = []               # test points img1
    test2  = []               # test points img2

    # separate into training and test sets:
    for i in range(len(pairs)):
        p1 = list(pairs[i][0])                  # point in img1: (x,y)
        p2 = list(pairs[i][1])                  # point in img2: (x,y)
        if i in subset:
            train1.append(p1)
            train2.append(p2)
        else:
            test1.append(p1)
            test2.append(p2)
    train1 = np.array(train1, dtype=np.float32)
    train2 = np.array(train2, dtype=np.float32)
    test1  = np.array(test1,  dtype=np.float32)
    test2  = np.array(test2,  dtype=np.float32)

    # create model
    # P is the transformation matrix for this projective transformation (apply it to img1 later)
    #P = cv2.getPerspectiveTransform(train1, train2)
    # calculate matrix manually instead of using getPerspectiveTransform():
    A = []
    B = []
    for i in range(0,4):
        p1 = train1[i]                          # point in img1
        p2 = train2[i]                          # point in img2
        A.append([p1[0], p1[1], 1, 0, 0, 0, -p2[0]*p1[0], -p2[0]*p1[1]])
        A.append([0, 0, 0, p1[0], p1[1], 1, -p2[1]*p1[0], -p2[1]*p1[1]])
        B.append(p2[0])
        B.append(p2[1])
    A = np.array(A, dtype=np.float)
    B = np.array([B], dtype=np.float).T
    # calculate coefficients:
    try:
        P = np.linalg.solve(A, B)
    except np.linalg.LinAlgError as err:
        if 'Singular matrix' in str(err):       # when matrix is not invertible
            P = np.linalg.lstsq(A, B)[0]        # approximates the solution
        else:
            raise
    # append the number 1.0 to the end and shape into a 3x3 matrix
    P = np.append(P.reshape(8), [1.0]).reshape((3,3))

    # apply the model to all the other points to evaluate error
    out = cv2.perspectiveTransform(np.array([test1]), P)[0]
    # get error (distance away from estimate)
    inliners = []
    totalError = 0                              # total squared error for the inliners
    for i in range(len(test1)):
        val = np.linalg.norm(test2[i]- out[i])**2
        if val < epsilon:
            totalError += val
            inliners.append(i)                   # index in test1

    return (P, inliners, totalError, test1, test2)


# create a model for the transformation using the best pairs
# return the final homography (image)
# (this is my implementation of cv2.findHomography)
# https://en.wikipedia.org/wiki/Random_sample_consensus#Algorithm
# TODO: see step4 here: http://www.cse.yorku.ca/~kosta/CompVis_Notes/ransac.pdf
#       perhaps repeat ransac recursively (1 more time) using just the set of inliners
def ransac(img1, img2, pairs):
    # create many random subsets (size=4 for projective transformation) of pairs
    sets = []
    count = 0                                   # iterations since last finding a new set
    while len(sets) < 10000:
        count += 1
        if count > 300:                         # stop if we're running out of new sets to generate
            break
        # each subset is list of indicies (of pairs)
        subset = random.sample(range(0, len(pairs)), 4)  # sample without replacement
        subset.sort()                                    # sort ascending
        if subset not in sets:
            sets.append(subset)
            count = 0

    epsilon = 8                                 # inliner if error < 8
    inliners = []                               # number of inliners for each of the models created using sets
    best = 0
    totals = []
    # create each model and compare
    for c in range(len(sets)):
        #print("\n\ncreating model " + str(c))
        res = createModel(pairs, sets[c], epsilon)
        inliners.append(res[1])
        totals.append(res[2])
        if len(inliners[c]) > len(inliners[best]):
            best = c

    # if multiple sets had the same number of inliners, pick the set with the lowest error on those inliners
    for c in range(len(sets)):
        if len(inliners[c]) == len(inliners[best]):
            if totals[c] < totals[best]:
                best = c

    # get the best (final) model
    res = createModel(pairs, sets[best], epsilon)
    H = res[0]
    inliners = res[1]
    print("best model:\n" + str(H))
    print("number of inliners: " + str(len(inliners)) + " out of " + str(len(pairs)))
    print("average residual:   " + str(res[2] / len(inliners)))
    test1 = np.int0(res[3])
    test2 = np.int0(res[4])

    # show the inliners on the images:
    icopy1 = np.copy(img1)
    icopy2 = np.copy(img2)
    for k in range(len(inliners)):
        i = inliners[k]
        # points are [x,y] so flip to be [row, col] (needed for array indexing)
        # mark a larger area around this pixel as red so we can see it (start:stop:step)
        icopy1[test1[i][1]-10:test1[i][1], test1[i][0]-10:test1[i][0]] = [0,0,255] 
        icopy2[test2[i][1]-10:test2[i][1], test2[i][0]-10:test2[i][0]] = [0,0,255] 
    # show the images side by side
    f, axarr = plt.subplots(1,2)
    axarr[0].imshow(cv2.cvtColor(np.copy(icopy1), cv2.COLOR_BGR2RGB))
    axarr[1].imshow(cv2.cvtColor(np.copy(icopy2), cv2.COLOR_BGR2RGB))
    plt.show()
    return H


# apply the projective transform H to img2 and then combine with img1 (translating as needed)
# (based on: https://stackoverflow.com/a/20355545)
def applyHomography(H, img1, img2):
    h1,w1 = img1.shape[:2]
    h2,w2 = img2.shape[:2]
    # corner locations in each image: (reshape and let np determine number of rows to fit data)
    pts1 = np.float32([[0,0],[0,h1],[w1,h1],[w1,0]]).reshape(-1,1,2)
    pts2 = np.float32([[0,0],[0,h2],[w2,h2],[w2,0]]).reshape(-1,1,2)

    # find the new corners of img2 after the transformation
    pts2_ = cv2.perspectiveTransform(pts2, H)
    # append the new corner points to the list of img1 corner points
    pts = np.concatenate((pts1, pts2_), axis=0)
    # get the min and max x and y indices within this set of points:
    xmin, ymin = np.int32(pts.min(axis=0).ravel() - 0.5)
    xmax, ymax = np.int32(pts.max(axis=0).ravel() + 0.5)

    # translation matrix to align img2 after it's transformed
    Ht = np.array([[1,0,-xmin],[0,1,-ymin],[0,0,1]])

    # perspective transform img2 and translate
    result = cv2.warpPerspective(img2, Ht.dot(H), (xmax-xmin, ymax-ymin))

    # combine the pixels of img1 with the transformed img2
    # take the average of overlapping pixels instead of overlaying
    for row in range(-ymin, h1+-ymin):
        for col in range(-xmin, w1+-xmin):
            # average pixels if pixel is present in both images
            if np.array_equal(result[row, col], [0,0,0]):
                result[row, col] = img1[row+ymin, col+xmin]
            else:
                result[row, col] = np.mean( np.array([ img1[row+ymin, col+xmin], result[row,col] ]), axis=0 )

    #result[-ymin:h1+-ymin, -xmin:w1+-xmin] = img1   # overlay img1 on top instead
    return result


def getDescriptorPairs(img1, img2):
    # convert to double and grayscale
    gray1 = np.float32(cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY))
    gray2 = np.float32(cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY))
    # get list of harris corner points for each image. note: these descriptors are actually [x,y] instead of [row, col]
    dst1 = getDescriptors(gray1, 200)            # get desired number of descriptors
    dst2 = getDescriptors(gray2, 200)

    # create a dict where the key is the tuple (index1, index2) of the indices of two descriptors
    # and the value is the euclidean distance between the descriptors' values:
    pairs = {}
    pairs2 = {}                                 # second best pairs
    for i in range(len(dst1)):
        p1 = dst1[i].ravel()                    # array [x,y] of descriptor location in img1
        d1 = neighborVector(gray1, p1, 10)      # vector of the descriptor (vector)

        # find the best match from set of descriptors in img2
        best = (((-1, -1), (-1, -1)), sys.float_info.max)   # index, value of best descriptor to pair with
        best2 = (((-1, -1), (-1, -1)), sys.float_info.max)  # second best
        for k in range(len(dst2)):
            p2 = dst2[k].ravel()
            d2 = neighborVector(gray2, p2, 10)
            if d1.shape != d2.shape:            # when one descriptor is too close to an edge for a full neighborhood
                continue
            val = np.linalg.norm(d1 - d2)       # compute the euclidian distance of these pairs
            if val < best[1]:
                best2 = best
                best = ((tuple(p1), tuple(p2)), val)
        # only keep pair if second best pair is not too similar
        if best[1]/best2[1] < 0.75:
            pairs[best[0]] = best[1]

    # sort the keys (pairs of points) into a list by size of their corresponding value
    sort = sorted(pairs, key=pairs.get)
    #return sort

    # visualize the pairs:
    icopy1 = np.copy(img1)
    icopy2 = np.copy(img2)
    for key in sort:                            # each element is a key in pairs
        # points are [x,y] so flip to be [row, col] (needed for array indexing)
        # mark a larger area around this pixel as red so we can see it (start:stop:step)
        pixel = [random.randint(0,256), random.randint(0,256), random.randint(0,256)]
        icopy1[key[0][1]-10:key[0][1], key[0][0]-10:key[0][0]] = pixel
        icopy2[key[1][1]-10:key[1][1], key[1][0]-10:key[1][0]] = pixel
    # show the images side by side
    f, axarr = plt.subplots(1,2)
    axarr[0].imshow(cv2.cvtColor(np.copy(icopy1), cv2.COLOR_BGR2RGB))
    axarr[1].imshow(cv2.cvtColor(np.copy(icopy2), cv2.COLOR_BGR2RGB))
    plt.show()
    return sort


# find up to maxQuantity descriptor points in an image
# (that are at least 8 pixels away from each other)
# img: np array image (grayscale)
# returns a list of the locations of the descriptors: [[[x1,y1]], [[x2,y2]], ...]
def getDescriptors(img, maxQuantity):
    return np.int0(cv2.goodFeaturesToTrack(img,maxQuantity,0.01,10, useHarrisDetector=True))


# takes the pixels around a target pixel and flattens them into a 1D vector
# img:   image array (grayscale)
# loc:   tuple indicating location of pixel in img (x,y) not (row, col)
# r:     size of area around pixel to look (e.g. 2 pixels in each direction)
# returns a 1D vector
def neighborVector(img, loc, r):
    # start:stop:end to get subsection of array
    return img[loc[1]-r:loc[1]+r+1,  loc[0]-r:loc[0]+r+1].flatten()

def showImage(img):
    # cv2 stores pixel colors in reverse order so convert:
    plt.imshow(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
    plt.show()


if __name__ == "__main__":
    main()
