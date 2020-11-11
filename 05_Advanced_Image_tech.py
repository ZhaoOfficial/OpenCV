import cv2
import numpy as np

timeout = 1

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
srcImage = cv2.imread("1.jpg", cv2.IMREAD_COLOR)
for i in range(3):
    drawColorHistogram(srcImage[:, :, i], i)

# equalized histogram of color
## looks like old image
(b, g, r) = cv2.split(srcImage)
bHist = cv2.equalizeHist(b)
gHist = cv2.equalizeHist(g)
rHist = cv2.equalizeHist(r)
dstImage = cv2.merge((bHist, gHist, rHist))
cv2.imshow("equalized HoC", dstImage)
cv2.waitKey(2000)





