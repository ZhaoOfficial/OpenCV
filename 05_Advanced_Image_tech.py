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
    minHeight, maxHeight, minCoor, maxCoor = cv2.minMaxLoc(result)
    histogramImage = np.zeros((512, 512, 3))
    for i in range(256):
        cv2.line(histogramImage, (2 * i, 512), (2 * i, 512 - int((512 / maxHeight) * result[i])), color, 2)
    cv2.imshow(windowName, histogramImage)
    cv2.waitKey(timeout)

for i in range(3):
    drawColorHistogramSelfMade(srcImage[:, :, i], i)


    