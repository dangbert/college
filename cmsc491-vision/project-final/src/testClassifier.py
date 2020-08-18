# Import the required modules
from skimage.transform import pyramid_gaussian
from skimage.io import imread
from skimage.feature import hog
from sklearn.externals import joblib
import cv2
import argparse as ap
from nms import nms
from config import *
import random
import matplotlib.pyplot as plt
from read_label_file import get_all_labels   # modified version of script from BOSCH traffic dataset github
DATA_DIR = "/home/dan/projects/cmsc491/Bosch_Traffic_Dataset"

def main():
    # Parse the command line arguments
    parser = ap.ArgumentParser()
    parser.add_argument('-i', "--image", help="Path to the test image", default=None)
    parser.add_argument('-n', "--num", help="number of random images to get from test set", default=None)
    parser.add_argument('-d','--downscale', help="Downscale ratio", default=1.25,
            type=int)
    parser.add_argument('-v', '--visualize', help="Visualize the sliding window",
            action="store_true")
    args = vars(parser.parse_args())

    # test on the provided image path
    if args['image'] != None:
        testImage(args['image'], args)

    # test on num images
    elif args['num'] != None:
        num = int(args['num'])
        samples = getTestSample(num, num)
        for info in samples:
            path = DATA_DIR + "/rgb_test/test/" + info['path']
            testImage(path, args)

# return list of num testData files to test
# requireLights: number of images you want to have a traffic light
#                default: don't consider number of lights
def getTestSample(num, requireLights=0):
    res = []
    testData = get_all_labels(DATA_DIR + "/rgb_test/test.yaml")
    indices = [i for i in range(len(testData))]
    random.shuffle(indices)
    for i in indices:
        info = testData[i]
        if requireLights <= 0:
            res.append(info)
        elif len(info['boxes']) > 0:
            res.append(info)
            requireLights -= 1
        if len(res) == num: # we have enough images
            return res

def sliding_window(image, window_size, step_size):
    '''
    This function returns a patch of the input image `image` of size equal
    to `window_size`. The first image returned top-left co-ordinates (0, 0) 
    and are increment in both x and y directions by the `step_size` supplied.
    So, the input parameters are -
    * `image` - Input Image
    * `window_size` - Size of Sliding Window
    * `step_size` - Incremented Size of Window

    The function returns a tuple -
    (x, y, im_window)
    where
    * x is the top-left x co-ordinate
    * y is the top-left y co-ordinate
    * im_window is the sliding window image
    '''
    for y in range(0, image.shape[0], step_size[1]):
        for x in range(0, image.shape[1], step_size[0]):
            yield (x, y, image[y:y + window_size[1], x:x + window_size[0]])

# test one image
# path: path to image file
def testImage(path, args):
    # Read the image
    #im = imread(args["image"], as_grey=False)
    im = cv2.imread(path, cv2.IMREAD_GRAYSCALE) # read img from file
    min_wdw_sz = (24, 48)
    step_size = (5, 5)
    downscale = args['downscale']
    visualize_det = args['visualize']

    # Load the classifier
    clf = joblib.load("../myData/traffic.model6")

    # List to store the detections
    detections = []
    # The current scale of the image
    scale = 0
    # Downscale the image and iterate
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
            fd = hog(im_window, orientations, pixels_per_cell, cells_per_block, block_norm="L1", visualise=False, transform_sqrt=True, feature_vector=True)
            fd = [fd] # clf wants a list of samples
            #pred = clf.predict(fd)
            probs = list(clf.predict_proba(fd)[0])
            #print(pred)
            pred = probs.index(max(probs)) # index of max
            # TODO: use all my labels
            if pred != 0 and probs[pred] > 0.80:
                print("prediction is " + str(pred))
                print(probs)
                #plt.imshow(im_window)
                #plt.show()
                print("Detection:: Location -> ({}, {})".format(x, y))
                print("Scale ->  {} | Confidence Score {} \n".format(scale, probs[pred]))
                detections.append((x, y, probs[pred],
                    int(min_wdw_sz[0]*(downscale**scale)),
                    int(min_wdw_sz[1]*(downscale**scale))))
                cd.append(detections[-1])
            # If visualize is set to true, display the working
            # of the sliding window
            if visualize_det:
                clone = im_scaled.copy()
                for x1, y1, _, _, _  in cd:
                    # Draw the detections at this scale
                    cv2.rectangle(clone, (x1, y1), (x1 + im_window.shape[1], y1 +
                        im_window.shape[0]), (0, 0, 0), thickness=2)
                cv2.rectangle(clone, (x, y), (x + im_window.shape[1], y +
                    im_window.shape[0]), (255, 255, 255), thickness=2)
                cv2.imshow("Sliding Window in Progress", clone)
                cv2.waitKey(30)
        # Move the the next scale
        scale+=1

    # Display the results before performing NMS
    clone = im.copy()
    for (x_tl, y_tl, _, w, h) in detections:
        # Draw the detections
        cv2.rectangle(im, (x_tl, y_tl), (x_tl+w, y_tl+h), (0, 0, 0), thickness=2)
    cv2.imshow("Raw Detections before NMS", im)
    cv2.waitKey()

    # Perform Non Maxima Suppression
    detections = nms(detections, threshold)

    # Display the results after performing NMS
    for (x_tl, y_tl, _, w, h) in detections:
        # Draw the detections
        cv2.rectangle(clone, (x_tl, y_tl), (x_tl+w,y_tl+h), (0, 0, 0), thickness=2)

    # save image to file
    fname = path[::-1] # reverse the string
    index = fname.find("/")
    fname = fname[:index][::-1]     # isolate just the file name and reverse back
    cv2.imwrite(fname, clone)
    print("saved to " + str(fname))
    cv2.imshow("Final Detections after applying NMS", clone)
    cv2.waitKey()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
