### Tinyimage
* ./main.py -d ../data/ -f tinyimage --tinyimage-patchdim 7 -c knn -k 5
* ./main.py -d ../data/ -f tinyimage --tinyimage-patchdim 6 -c svm -l 1.0

### bow-patches
* ./main.py -d ../data/ -f bow-patches --patches-dictionarysize 128 --patches-radius 8 --patches-stride 11  -c knn -k 7 
* ./main.py -d ../data/ -f bow-patches --patches-dictionarysize 60 --patches-radius 10 --patches-stride 6  -c svm -l 110

### bow-sift
* ./main.py -d ../data/ -f bow-sift --sift-dictionarysize 128 --sift-binsize 15 --sift-radius 8 --sift-stride 12 -c knn -k 5
* ./main.py -d ../data/ -f bow-sift --sift-dictionarysize 128 --sift-binsize 10 --sift-radius 8 --sift-stride 12 -c svm -l 1.0



### Misc Code:
* img = cv2.imread("test.jpg", cv2.IMREAD_GRAYSCALE)   # read image in grayscale
* kp, des = sift.detectAndCompute(img,None)   # get keypoints and calculate descriptors
* showImage(cv2.drawKeypoints(img,kp, np.array([]), flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS))
