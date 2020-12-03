import cv2
import numpy as np

timeout = 1
srcImage = cv2.imread("test.jpg", cv2.IMREAD_COLOR)
H = srcImage.shape[0]
W = srcImage.shape[1]

def drawColorHistogram(image, channel):
    if channel == 0:
        windowName = 'b histogram'
        color = (255, 0, 0)
    if channel == 1:
        windowName = 'g histogram'
        color = (0, 255, 0)
    if channel == 2:
        windowName = 'r histogram'
        color = (0, 0, 255)

    result = cv2.calcHist([image], [0], None, [256], [0.0, 255.0])
    minHeight, maxHeight, minCoor, maxCoor = cv2.minMaxLoc(result)
    histogramImage = np.zeros((512, 512, 3))
    for i in range(256):
        cv2.line(histogramImage, (2 * i, 512), (2 * i, 512 - int((512 / maxHeight) * result[i])), color, 2)
    cv2.imshow(windowName, histogramImage)
    cv2.waitKey(timeout)

# histogram of color
for i in range(3):
    drawColorHistogram(srcImage[:, :, i], i)

# equalized histogram of color
## looks like old image
## b, g, r
(b, g, r) = cv2.split(srcImage)
bHist = cv2.equalizeHist(b)
gHist = cv2.equalizeHist(g)
rHist = cv2.equalizeHist(r)
dstImage = cv2.merge((bHist, gHist, rHist))
cv2.imshow("equalized HoC", dstImage)
cv2.waitKey(timeout)


## YUV
imageYUV = cv2.cvtColor(srcImage, cv2.COLOR_BGR2YCrCb)
cv2.imshow("YUV", imageYUV)
cv2.waitKey(timeout)
channelYUV = cv2.split(imageYUV)
channelYUV[0] = cv2.equalizeHist(channelYUV[0])
channels = cv2.merge(channelYUV)
dstImage = cv2.cvtColor(channels, cv2.COLOR_YCrCb2BGR)
cv2.imshow("equalized YUV", dstImage)
cv2.waitKey(timeout)


# image repairing
dstImage = srcImage
dstImage[200:205, 250:255] = [255, 255, 255]
cv2.imshow('damaged', dstImage)
cv2.waitKey(timeout)
mask = np.zeros((H, W, 1), np.uint8)
mask[200:205, 250:255] = 255
dstImage = cv2.inpaint(dstImage, mask, 3, cv2.INPAINT_TELEA)
cv2.imshow('repaired', dstImage)
cv2.waitKey(timeout)



# histogram of color self made
def drawColorHistogramSelfMade(image, channel):
    if channel == 0:
        windowName = 'b histogram self made'
        color = (255, 0, 0)
    if channel == 1:
        windowName = 'g histogram self made'
        color = (0, 255, 0)
    if channel == 2:
        windowName = 'r histogram self made'
        color = (0, 0, 255)

    count = np.zeros(256, np.uint16)
    for i in range(H):
        for j in range(W):
            count[srcImage[i, j, channel]] += 1

    result = cv2.calcHist([image], [0], None, [256], [0.0, 255.0])
    maxHeight = cv2.minMaxLoc(result)[1]
    histogramImage = np.zeros((512, 512, 3))
    for i in range(256):
        cv2.line(histogramImage, (2 * i, 512), (2 * i, 512 - int((512 / maxHeight) * result[i])), color, 2)
    cv2.imshow(windowName, histogramImage)
    cv2.waitKey(timeout)

# for i in range(3):
#     drawColorHistogramSelfMade(srcImage[:, :, i], i)



# equalized histogram of color self made
def equalizedHistogramSelfMade(image:np.ndarray):
    H = image.shape[0]
    W = image.shape[1]

    count_b = np.zeros(256, np.float32)
    count_g = np.zeros(256, np.float32)
    count_r = np.zeros(256, np.float32)

    for i in range(H):
        for j in range(W):
            (b, g, r) = image[i, j]
            count_b[b] += 1
            count_g[g] += 1
            count_r[r] += 1


    count_b /= (H * W)
    count_g /= (H * W)
    count_r /= (H * W)

    for i in range(1, 256):
        count_b[i] += count_b[i - 1]
        count_g[i] += count_g[i - 1]
        count_r[i] += count_r[i - 1]

    map_b = count_b * 255
    map_g = count_g * 255
    map_r = count_r * 255

    dstImage = np.zeros((H, W, 3), dtype = np.uint8)

    for i in range(H):
        for j in range(W):
            (b, g, r) = image[i, j]
            b = map_b[b]
            g = map_g[g]
            r = map_r[r]
            dstImage[i, j] = (b, g, r)
    cv2.imshow('equalized dstImage', dstImage)
    cv2.waitKey(timeout)

#equalizedHistogramSelfMade(srcImage)



# enhance the lightness
dstImage = srcImage
dstImage = dstImage.astype(np.float32)
dstImage += 40
dstImage[dstImage > 255] = 255
dstImage = dstImage.astype(np.uint8)
cv2.imshow('enhanced', dstImage)
cv2.waitKey(timeout)



# bilateral filter
# mopi
dstImage = cv2.bilateralFilter(srcImage, 15, 35, 35)
cv2.imshow('bilateral', dstImage)
cv2.waitKey(timeout)



# Gaussian filter
# param: image, size, std
dstImage = cv2.GaussianBlur(srcImage, (3, 3), 1)
cv2.imshow('gaussian', dstImage)
cv2.waitKey(timeout)


# mean filter
def meanFilter(image, size):

    H = image.shape[0]
    W = image.shape[1]

    dstImage = np.zeros((H - size, W - size, 3), dtype = np.uint8)

    for i in range(0, H - size):
        for j in range(0, W - size):
            (b, g, r) = np.mean(image[i:i + size, j:j + size], axis = (0, 1))
            dstImage[i, j] = (b, g, r)
    cv2.imshow('mean', dstImage)
    cv2.waitKey(timeout)

meanFilter(srcImage, 6)



# median filter
def medianFilter(image, size):

    H = image.shape[0]
    W = image.shape[1]

    dstImage = np.zeros((H - size, W - size, 3), dtype = np.uint8)

    for i in range(0, H - size):
        for j in range(0, W - size):
            (b, g, r) = np.median(image[i:i + size, j:j + size], axis = (0, 1))
            dstImage[i, j] = (b, g, r)
    cv2.imshow('median', dstImage)
    cv2.waitKey(4000)

medianFilter(srcImage, 6)