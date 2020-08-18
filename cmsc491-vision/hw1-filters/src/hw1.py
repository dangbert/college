#!/usr/bin/env python3
# Dan Engbert
# Computer Vision - Spring 2018
import numpy as np
from PIL import Image                           # (pip3 install pillow)
import scipy.stats as st                        # st.norm
import scipy.ndimage.filters as filter
from matplotlib import pyplot as plt
import copy

# for negatives you can divide pixel values by 2 and add 128
# (-255, 255) -> (0, 255)
# you could also just take the absolute value but then you lose some information
def main():

    # test gaussian blur:
    arr1 = np.array(Image.open("data/Seattle.jpg"))
    Image.fromarray(GaussianBlurImage(arr1, 4.0)).save("1.png")
    # test seperable gaussian blur:
    Image.fromarray(SeparableGaussianBlurImage(arr1, 4.0)).save("2.png")

    # test 1st derivative
    arr2 = np.array(Image.open("data/LadyBug.jpg"))
    Image.fromarray(FirstDerivImage(arr2, 1.0)).save("3a.png")
    # test 2nd derivative
    Image.fromarray(SecondDerivImage(arr2, 1.0)).save("3b.png")

    # test SharpenImage
    arr3 = np.array(Image.open("data/Yosemite.png"))
    #arr3 = arr3[:,:,:3]
    Image.fromarray(SharpenImage(arr3, 1.0, 5.0)).save("4.png")

    # test Sobel filter
    arr4 = np.array(Image.open("data/LadyBug.jpg"))
    results = SobelImage(arr4)
    Image.fromarray(results[0]).save("5a.png")  # magnitude of edges
    Image.fromarray(results[1]).save("5b.png")  # direction of edges

    # test scaling with BilinearInterpolation
    arr5 = np.array(Image.open("data/Moire_small.jpg"))
    Image.fromarray(ScaleImageBilinear(arr5, 4.0, "neighbor")).save("6a.png")
    Image.fromarray(ScaleImageBilinear(arr5, 4.0, "bilinear")).save("6b.png")

    #plt.imshow(arr2)
    #plt.show()

# gaussain blur an image using a 2D kernel
# arr:    np array representing the image
# sigma:  float (standard deviation of blur)
def GaussianBlurImage(arr, sigma):
    fsize = int(4*sigma) + 1                    # filter size
    # generate 2D gaussian kernel
    kernel = np.zeros((fsize, fsize), dtype=np.float32)
    for i in range(fsize):
        for j in range(fsize):
            # make the center of the image the origin
            x = i - (fsize-1) / 2
            y = j - (fsize-1) / 2
            kernel[i, j] = 1.0 / (2 * np.pi * sigma**2) * np.exp(-(x**2 + y**2) / (2 * sigma**2))
    # for gaussian we don't have to divide the sum by anything because the kernel already adds to 1
    arr = applyFilter(arr, kernel).astype('uint8')
    return arr

# gaussian blur an image using two 1D kernels
# (faster than GaussianBlurImage)
# arr:    np array representing the image
# sigma:  float (standard deviation of blur)
def SeparableGaussianBlurImage(arr, sigma):
    fsize = int(4*sigma) + 1                    # filter size

    # generate 1D gaussian kernels (horizontal and vertical)
    kernelh = np.zeros((1, fsize), dtype=np.float32)
    for i in range(fsize):
        x = i - (fsize-1) / 2                   # the center of the array is the origin
        kernelh[0, i] = 1.0 / (np.sqrt(2 * np.pi) * sigma) * np.exp(-(x**2) / (2 * sigma**2))
    kernelv = kernelh.transpose()

    # apply gaussian filter in horizontal and then in vertical direction
    arr = applyFilter(arr, kernelh).astype('uint8')
    return applyFilter(arr, kernelv).astype('uint8')

# compute 1st derivative of an image (in x-direction)
# then perform gaussian blur
# arr:    np array representing the image
# sigma:  float (standard deviation of blur)
def FirstDerivImage(arr, sigma):
    kernel = np.array([[-1, 0, 1,]])
    # apply a horizontal gradient and add 128 (to account for negative values)
    arr = applyFilter(arr, kernel, 1, 128).astype('uint8')
    # gaussian blur the resulting image
    return SeparableGaussianBlurImage(arr, sigma)

# compute 2nd derivative of an image (in x and y directions)
# then perform gaussian blur
# arr:    np array representing the image
# sigma:  float (standard deviation of blur)
def SecondDerivImage(arr, sigma):
    # TODO: my output has a few artifacts (see note in applyFilter function)
    # figure out what to do about negative pixels
    kernel = np.array([[0, 1, 0], [1, -4, 1], [0, 1, 0]])
    # divide final pixels values by 2 and add 128 to account for negatives
    arr = applyFilter(arr, kernel, 0.5, 128).astype('uint8')
    arr = SeparableGaussianBlurImage(arr, sigma)
    return arr

# sharpen an image
# arr:    np array representing the image
# sigma:  float (standard deviation of blur)
# alpha:  scale factor
def SharpenImage(arr, sigma, alpha):
    arr2 = SecondDerivImage(arr, sigma)
    #plt.imshow(arr2)
    #plt.show()
    # TODO: perhaps if something is out of range (0,255), just change it to be 0 or 255 (whichever is closer)
    #result = (arr - (arr2 - 128) * alpha)
    #result = np.zeros(arr.shape, dtype=np.float32)
    plt.imshow(arr2)
    plt.show()
    result = (arr2-128)
    result = arr - (arr2-128)
    min = result[0,0,0]
    max = result[0,0,0]
    for y in range(len(result)):
        for x in range(len(result[0])):
            for c in range(0, len(result[0,0])):
                val = result[y,x][c]
                if val < min:
                    min = val
                if val > max:
                    max = val
    plt.imshow(result)
    plt.show()
    print("min = " + str(min) + ", max=" + str(max))
    return

    result = arr - ((arr2-128) * alpha)
    plt.imshow(result)
    plt.show()

    min = result[0,0,0]
    max = result[0,0,0]

    for y in range(len(result)):
        for x in range(len(result[0])):
            for c in range(0, len(result[0,0])):
                val = result[y,x][c]
                if val < min:
                    min = val
                if val > max:
                    max = val
                if val < 0:
                    #print('val was ' + str(val))
                    val = 0
                if val > 255:
                    #print('val was ' + str(val))
                    val = 255 
                #print('**val was good')
                #result[y,x][c] = val
    print("min = " + str(min) + ", max=" + str(max))
    result = result.astype('uint8')
    # TODO: convert to ints
    plt.imshow(result)
    plt.show()
    return result

# perform the sobel filter on an image
# show the magnitude and orientation of the edges in the image
# arr:    np array representing the image
# return a tuple of images:
#   one image showing the magnitude of the edge
#   and one image showing the direction
# https://en.wikipedia.org/wiki/Sobel_operator#Example
def SobelImage(arr):
    # gaussian blur first to remove some noise
    arr = SeparableGaussianBlurImage(arr, 2.0)
    # TODO: there are still some extra artifacts that show up
    # TODO: the direction image is wrong
    # convert to greyscale (1D pixels)
    colors = copy.deepcopy(arr)                 # will preserve PNG opacity later
    arr = np.array(Image.fromarray(arr).convert('L'))
    kernelx = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
    kernely = kernelx.transpose()

    arrx = applyFilter(arr, kernelx)
    arry = applyFilter(arr, kernely)
    mag = np.zeros(arr.shape, dtype=np.uint8)   # magnitude of vector
    # now create an image with each pixel given by = sqrt(arrx^2 + arry^2)
    # and another image where the color of each pixel is mapped to the angle of the edge
    for y in range(len(arrx)):
        for x in range(len(arrx[0])):
            # the way you get current pixel (x, y) is backwards: arr[y, x]
            # calculate the magnitude and scale down to be <= 255
            mag[y,x] = (np.sqrt(arrx[y,x]**2 + arry[y,x]**2)) * 255/360
            # can't divide by 0
            if arrx[y,x] == 0:
                colors[y,x][0] = 0
                colors[y,x][1] = 0
                colors[y,x][2] = 0
                continue
            # arctan returns a value between -pi and pi
            # for the direction image, map 180 degrees to the spectrum of RGB colors [0,0,0] -> [255, 255, 255]
            angle = np.arctan(arry[y,x] / arrx[y,x]) + np.pi/2
            val = (angle/np.pi * 256**3)        # multiply ratio by 256^3
            # now convert val to an RGB pixel (base 256)
            colors[y,x][0] = val / 256**2
            val = val % 256**2
            colors[y,x][1] = val / 256
            colors[y,x][2] = val % 256
            #print(str(angle) + " -> " + str(colors[y,x]))

    # convert back to positive integer pixels
    mag = mag.astype('uint8')
    # convert back to color
    mag = np.array(Image.fromarray(mag).convert('RGB'))
    return (mag, colors)

# find the peak edge responses perpendicular to the edges
# arr:    np array representing the image
# thres:  threshold a pixe must be larger than to be a peak response
def FindPeaksImage(arr, thres):
# TODO: this function doesn't work
    sob = SobelImage(arr)
    mags = sob[0]                               # edge magnitudes
    dirs = sob[1]                               # edge directions
    res = np.zeros(arr.shape, dtype=np.uint)
    for y in range(len(arr)):
        for x in range(len(arr[0])):
            # check pixel vs neighbors
            m = mags[y,x] # edge magnitude
            aPixel = dirs[y,x] 
            a = 0 # angle
            for c in range(len(aPixel)):
                a += 255**(2-c) * aPixel[c]
            a = float(a / 256**3) * np.pi        # angle: (radians)
            dx = np.cos(a)
            dy = np.sin(a)
            nx = x + dx
            ny = y + dy
            # TODO: what if we exceed the boundary?
            if nx > len(arr[0]):
                nx = len(arr[0])-1
            if nx < 0:
                nx = 0
            if ny > len(arr):
                ny = len(arr)-1
            if ny < 0:
                ny = 0
            e0 = BilinearInterpolation(arr, nx, ny)

            nx = x - dx
            ny = y - dy
            if nx > len(arr[0]):
                nx = len(arr[0])-1
            if nx < 0:
                nx = 0
            if ny > len(arr):
                ny = len(arr)-1
            if ny < 0:
                ny = 0
            e1 = BilinearInterpolation(arr, nx, ny)
            if mags[y,x][0] > thres and mags[y,x][0] > e0[0] and mags[y,x][0] > e1[0]:
                res[y,x] = np.array([255, 255, 255])
    return res


# returns the linearly interpolated pixel at (x, y)
# arr:    np array representing the image
# x:      float representing the x-value of a point on the image
# y:      float representing the y-value of a point on the image
def BilinearInterpolation(arr, x, y):
    psize = len(arr[0,0])
    pixel = np.zeros(psize, dtype=np.uint8)     # magnitude of vector
    # look at nearest 4 pixels
    x1 = int(x)
    y1 = int(y)
    edgex = False if (int(x+1) < len(arr[0])) else True
    edgey = False if (int(y+1) < len(arr)) else True
    x2 = int(x+1) if not edgex else int(x)
    y2 = int(y+1) if not edgey else int(y)
    # if an index is out of bounds (i.e. the right or bottom edge of the image) then use the pixel next to it
    p11 = arr[y1, x1]
    p12 = arr[y1, x2]
    p21 = arr[y2, x1]
    p22 = arr[y2, x2]
    # if at an edge, keep the real values of x2 and y2 for the calculations below
    if edgex:
        x2 = x2+1
    if edgey:
        y2 = y2+1

    # iterate over pixel components (skip opacity if it exists)
    for c in range(psize):
        if c == 3:
            break
        res = np.array([x2-x, x-x1]) / ((x2-x1) * (y2-y1))
        # matrix multiply
        res = np.dot(res,  np.array([[p11[c], p12[c]], [p21[c], p22[c]]]))
        res = np.dot(res,  np.array([[y2-y], [y-y1]]))
        pixel[c] = res[0]
    return pixel

# returns the nearest-neighbor interpolated pixel at (x, y)
# arr:    np array representing the image
# x:      float representing the x-value of a point on the image
# y:      float representing the y-value of a point on the image
def NeighborInterpolation(arr, x, y):
    x = int(np.rint(x))
    y = int(np.rint(y))
    if int(x+1) >= len(arr[0]):
        x -= 1
    if int(y+1) >= len(arr):
        y -= 1
    return arr[y,x]

# scales an image using BilinearInterpolation or nearest-neighbor
# arr:    np array representing the image
# scalr:  float value to scale the dimensions by
# mode:   string "bilinear" or "neighbor" to select the interpolation mode
# TODO: look up to check this is the correct way of doing (for bilinear interpolation)
def ScaleImageBilinear(arr, scalar, mode="bilinear"):
    height = len(arr)                           # image dimensions
    width = len(arr[0])
    shape = (int(arr.shape[0]*scalar), int(arr.shape[1]*scalar), arr.shape[2])
    arr2 = np.zeros(shape, dtype=np.uint8)      # new image
    for y in range(len(arr2)):
        for x in range(len(arr2[0])):
            if x % int(scalar) == 0 and y % int(scalar) == 0:
                arr2[y, x] = arr[int(y/scalar), int(x/scalar)]
            else:
                # interpolate using the specified mode
                if mode == "neighbor":
                    arr2[y,x] = NeighborInterpolation(arr, x/scalar, y/scalar)
                else:
                    arr2[y,x] = BilinearInterpolation(arr, x/scalar, y/scalar)

    return arr2

# apply a filter to an image
# returns an np.array of floats (possibly with negatives)
# convert back to an 'image' array later with: arr2.astype('uint8')
#
# arr:    np array of pixels (up to 4D)
# kernel: np array of values for filter
# scalar: scalar value to apply to array at end
# offset: value to add to array elements (after scalar is applied)
def applyFilter(arr, kernel, scalar=1, offset=0):
    height = len(arr)                           # image dimensions
    width = len(arr[0])
    ysize = len(kernel)                         # kernel dimensions
    xsize = len(kernel[0])
    psize = arr[0,0].size                       # pixel size (3 for RGB)
    # output array will be the same dimensions, but will be floating point values
    # (if we used an array of 'unit8', numpy would automatically add 256 to negative pixels)
    arr2 = np.zeros(arr.shape, dtype=np.float32)

    # iterate over pixels in arr (pixels are [R, G, B])
    for y in range(height):
        for x in range(width):
            # the way you get current pixel (x, y) is backwards: arr[y, x]

            # resulting pixel (so array can be up to 4D, like PNGs)
            totals = np.array([0.0, 0.0, 0.0, 0.0])
            # apply kernel to the area centered around this pixel (zero-pad if pixel doesn't exist)
            for j in range(ysize):
                dy = int(j - (ysize-1) / 2)     # y offset
                for i in range(xsize):
                    dx = int(i - (xsize-1) / 2) # x offset
                    if (x+dx) >= 0 and (x+dx) < width and (y+dy) >= 0 and (y+dy) < height:
                        cpixel = arr[y+dy, x+dx]
                    else:
                        cpixel = np.array([0, 0, 0, 0])
                    # iterate over dimensions of each pixel
                    for c in range(psize):
                        if cpixel.size == 1:
                            totals[0] += float(cpixel * kernel[j,i])
                            break
                        # don't operate on a PNGs transarency
                        if c <= 2:
                            totals[c] += float(cpixel[c] * kernel[j,i])

            # remove extra columns in totals that weren't needed (i.e. image is < 4D)
            totals = totals[:psize]
            # apply scalar and then offset (can be used to deal with negatives)
            totals = totals * scalar + offset
            arr2[y, x] = totals                 # write the new pixel

    return arr2

if __name__ == "__main__":
    main()
