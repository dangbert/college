#!/usr/bin/env python3
import sys
import argparse
import cv2
from matplotlib import pyplot as plt


import numpy as np
from sklearn.model_selection import KFold


from dataset import read_dataset
from features import compute_features
from classifier import train_classifier, make_predictions, show_confusion, get_confusion


CLASSIFIERS = ['knn', 'svm']
FEATURES = ['tinyimage', 'bow-patches', 'bow-sift', 'gist']


def main(args):
    parser = argparse.ArgumentParser(
        description='Train and evaluate a model on the Cats vs. Dogs dataset')

    parser.add_argument('-d', '--dataset-dir', required=True, type=str,
                        help='Path to the dataset')
    parser.add_argument('-f', '--feature', required=True, choices=FEATURES,
                        help='Select which feature representation to use. '
                        'Choices are {' + ', '.join(FEATURES) + '}')
    parser.add_argument('-c', '--classifier', required=True,
                        choices=CLASSIFIERS,
                        help='Select which classifier to use. '
                        'Choices are {' + ', '.join(CLASSIFIERS) + '}')
    parser.add_argument('-k', '--knn-k', default=3, type=int,
                        help='Number of neighbors for kNN classifier')
    parser.add_argument('-l', '--svm-lambda', default=1.0, type=float,
                        help='Lambda paramter for SVM')
    parser.add_argument('--tinyimage-patchdim', default=16, type=int)
    parser.add_argument('--patches-dictionarysize', default=128, type=int)
    parser.add_argument('--patches-radius', default=8, type=int)
    parser.add_argument('--patches-stride', default=12, type=int)
    parser.add_argument('--sift-dictionarysize', default=128, type=int)
    parser.add_argument('--sift-binsize', default=8, type=int,
                        help='Size of the bin in terms of number of pixels in '
                        'the image. Recall that SIFT has 4x4=16 bins.')
    parser.add_argument('--sift-radius', default=8, type=int)
    parser.add_argument('--sift-stride', default=12, type=int,
                        help='Spacing between succesive x (and y) coordinates '
                        'for sampling dense features.')
    parser.add_argument('--cross', default=0, type=int,
                        help='value of folds to use for k-fold crossvalidation. 0 for no validation')

    args = parser.parse_args(args)

    imdb = read_dataset(args.dataset_dir)

    # script to run overnight: (grep the lines with the accuracy afterwards)
    #sys.stdout = open('all_output.txt', 'w')
    """
    count = 0
    for dictsize in [128, 95, 160, 60, 200, 40, 25]:
        for k in [3, 12, 5, 7, 9]:
            for stride in [4, 7, 11]:
                for radius in [5, 8, 11]:
                    count += 1
                    args.patches_dictionarysize = dictsize
                    args.patches_radius = radius
                    args.patches_stride = stride
                    args.knn_k = k
                    print("\n\n***********testing params:*************")
                    print("dictsize: " + str(args.patches_dictionarysize))
                    print("radius:\t" + str(args.patches_radius))
                    print("stride:\t" + str(args.patches_stride))
                    print("knn_k\t" + str(args.knn_k))
                    sys.stdout.flush()
                    crossValidation(imdb, args)
        print('total trials ' + str(count))
    print('finished')
    return
    """

    if args.cross > 0:
        print("\n\n-----Doing cross validation:-----")
        crossValidation(imdb, args)
    else:
        print("\n\n------Running on test set:------")
        process(imdb, args, validation=False)


# train and test classifier
# validation: whether or not we are doing validation testing
#             if so don't use the test set and return a tuple (conf_mat, accuracy)
def process(imdb, args, validation=False):

    if validation:
        # test on the validation set
        features = compute_features(imdb, args, useValSet=False)
        print('Experiment setup: trainining set: train, test set: val')
        clf = train_classifier(
            features[imdb.train_indices, :],        # get rows corresponding to training
            imdb.class_ids[imdb.train_indices],
            args)
        val_preds, val_scores = make_predictions(
            clf,
            features[imdb.val_indices, :])
        if validation:
            return get_confusion(imdb.class_ids[imdb.val_indices], val_preds)
        #show_confusion(imdb.class_ids[imdb.val_indices], val_preds)

    else:
        features = compute_features(imdb, args, useValSet=True)
        # ensure that indices haven't been accidentally modified:
        assert imdb.train_indices[0] == 0 and imdb.train_indices[-1] == 297
        assert imdb.val_indices[0] == 1 and imdb.val_indices[-1] == 298
        assert imdb.test_indices[0] == 2 and imdb.test_indices[-1] == 299
        print('Experiment setup: trainining set: train+val, test set: test')
        clf = train_classifier(
            features[np.hstack((imdb.train_indices, imdb.val_indices)), :],
            imdb.class_ids[np.hstack((imdb.train_indices, imdb.val_indices))],
            args)
        test_preds, test_scores = make_predictions(
            clf,
            features[imdb.test_indices, :])
        show_confusion(imdb.class_ids[imdb.test_indices], test_preds)

        # confusion matrix of images: (store their indices in imdb.test_indices)
        # find first cat and first dog:
        cat, dog = -1, -1
        for i in range(len(imdb.test_indices)): # location in imdb.test_indices
            if cat == -1 and imdb.class_ids[imdb.test_indices[i]] == 0:
                cat = i
            if dog == -1 and imdb.class_ids[imdb.test_indices[i]] == 1:
                dog = i
        top = np.array([[cat,cat], [dog, dog]])
        for i in range(len(imdb.test_indices)): # location in imdb.test_indices
            # cat: 0, dog: 1 (labels)
            ans = imdb.class_ids[imdb.test_indices[i]]
            pred = test_preds[i]
            score = test_scores[i]
            if ans == 0 and pred == 0:          # look for most cat-like cat
                if score > test_scores[top[0,0]]:
                    top[0,0] = i
            if ans == 0 and pred == 1:          # look for most dog-like cat
                if score > test_scores[top[0,1]]:
                    top[0,1] = i
            if ans == 1 and pred == 1:          # look for most dog-like dog
                if score > test_scores[top[1,1]]:
                    top[1,1] = i
            if ans == 1 and pred == 0:          # look for most cat-like dog
                if score > test_scores[top[1,0]]:
                    top[1,0] = i
        # show the top images side by side
        fig, axarr = plt.subplots(2,2, figsize=(5,5))
        for i in range(0,2):
            for j in range(0,2):
                img = cv2.imread(imdb.image_dir + "/" + imdb.image_names[imdb.test_indices[top[i,j]]])
                axarr[i,j].imshow(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
        fig.savefig("confusion.jpg", dpi=fig.dpi)
        plt.show()


# perform k folds cross validation by changing imdb train and val indices
def crossValidation(imdb, args):
    k = args.cross
    orig_train = imdb.train_indices.copy()
    orig_val = imdb.val_indices.copy()
    fullSet = np.concatenate((imdb.train_indices, imdb.val_indices))

    # do k folds cross validation
    avg = 0
    kf = KFold(n_splits=k, shuffle=True)
    for train_index, test_index in kf.split(fullSet):
        imdb.train_indices = fullSet[train_index] # set for training
        imdb.val_indices = fullSet[test_index]    # set for validating
        conf_mat, accuracy = process(imdb, args, validation=True)
        avg += accuracy
        print(conf_mat)
        print("\taccuracy is: " + str(accuracy))

        #print(imdb.train_indices)
        #print(imdb.val_indices)
    avg = float(avg / k)
    print("\nAVERAGE ACCURACY OVER: " + str(k) + " folds " + str(avg))
    sys.stdout.flush()
    imdb.train_indices = orig_train
    imdb.val_indices = orig_val

if __name__ == '__main__':
    main(sys.argv[1:])
