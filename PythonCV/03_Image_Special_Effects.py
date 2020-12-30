import cv2
import numpy as np
from scipy.ndimage.filters import convolve
from skimage import filters

timeout = 1

# color -> gray
srcImage = cv2.imread('D:/Mywork/OpenCV/test.jpg', cv2.IMREAD_COLOR)
H, W, C = srcImage.shape
grayImage = cv2.cvtColor(srcImage, cv2.COLOR_BGR2GRAY)
cv2.imshow('gray1', grayImage)
cv2.waitKey(timeout)

## gray = (r + g + b) / 3
grayImage1 = np.sum(srcImage, axis = 2) / 3
grayImage1 = np.uint8(grayImage1)
cv2.imshow('gray2', grayImage1)
cv2.waitKey(timeout)

## gray = 0.299 * r + 0.587 * g + 0.114 * b
grayImage2 = srcImage[:, :, 0] * 0.114 + srcImage[:, :, 1] * 0.587 + srcImage[:, :, 2] * 0.299
grayImage2 = np.uint8(grayImage2)
cv2.imshow('gray3', grayImage2)
cv2.waitKey(timeout)


# color -> reversed color
reversedGrayImage = 255 - grayImage
cv2.imshow('reversed gray', reversedGrayImage)
cv2.waitKey(timeout)

reversedColorImage = 255 - srcImage
cv2.imshow('reversed color', reversedColorImage)
cv2.waitKey(timeout)


# mosaic
dstImage = srcImage.copy()
for h in range(200, 400):
    for w in range(200, 300):
        if w % 10 == 0 and h % 10 == 0:
            dstImage[h:h + 10, w:w + 10] = srcImage[h, w]
cv2.imshow('mosaic', dstImage)
cv2.waitKey(timeout)


# blur window
dstImage = srcImage.copy()
radius = 3
for h in range(radius, H - radius):
    for w in range(radius, W - radius):
        choose = np.random.randint(-radius + 1, radius)
        dstImage[h, w] = srcImage[h + choose, w + choose]
cv2.imshow('blur window', dstImage)
cv2.waitKey(timeout)


# weighted add
otherImage = cv2.imread('D:/Mywork/OpenCV/road.jpg', cv2.IMREAD_COLOR)
### param(image1, weight1, image2, weight2, gamma)
### Sizes of input arguments should match
dstImage = cv2.addWeighted(srcImage[:400, :400], 0.5, otherImage[:400, :400], 0.5, gamma = 0)
cv2.imshow('weighted add', dstImage)
cv2.waitKey(timeout)


# Edge detection
## Canny
### param(image, shape, std)
smoothedImage = cv2.GaussianBlur(grayImage, (3, 3), 1)
### param(image, threshold1, threshold2) the smaller threshold will be the lower bound and ...
dstImage = cv2.Canny(smoothedImage, 50, 100)
cv2.imshow('edge', dstImage)
cv2.waitKey(timeout)

## Sobel
### convolve, gradient, threshold
Gx = filters.sobel_h(grayImage)
Gy = filters.sobel_v(grayImage)
G = np.sqrt(Gx * Gx + Gy * Gy)
G[G > 100 / 255] = 255
G[G < 50 / 255] = 0
### non maximum suppression
### hysteresis
cv2.imshow('edge2', G)
cv2.waitKey(timeout)


# relief
matrix = np.array([[0, 0, 0], [-1, 1, 0], [0, 0, 0]])
dstImage = convolve(grayImage, matrix)
dstImage += 150
dstImage[dstImage > 255] = 255
cv2.imshow('relief', dstImage)
cv2.waitKey(timeout)


# bad enhance
dstImage = srcImage
dstImage = dstImage.astype(np.float32)
dstImage *= 1.2
dstImage[dstImage > 255] = 255
dstImage = np.uint8(dstImage)
cv2.imshow('enhance', dstImage)
cv2.waitKey(timeout)


# oil painting
# very very slow 480 * 640: 2 min
dstImage = np.zeros((srcImage.shape[0], srcImage.shape[1], 3))
for i in range(4, srcImage.shape[0] - 4):
    print(i)

    zero = np.zeros(8, dtype = np.uint8)
    for m in range(-4, 4):
        for n in range(-4, 3):
            intensity = grayImage[i + m, 4 + n] // 32
            zero[intensity] += 1
    for m in range(-4, 4):
        intensity = grayImage[i + m, -1] // 32
        zero[intensity] += 1

    for j in range(4, srcImage.shape[1] - 4):
        for m in range(-4, 4):
            intensity = grayImage[i + m, j - 5] // 32
            zero[intensity] -= 1
        for m in range(-4, 4):
            intensity = grayImage[i + m, j + 3] // 32
            zero[intensity] += 1

        currentMax = 0
        index = 0
        for k in range(8):
            if currentMax < zero[k]:
                currentMax = zero[k]
                index = k

        (b, g, r) = (0, 0, 0)
        count = 0
        for m in range(-4, 4):
            for n in range(-4, 4):
                if (index * 32) <= grayImage[i + m, j + n] <= (index * 32 + 32):
                    b += srcImage[i + m, j + n, 0]
                    g += srcImage[i + m, j + n, 1]
                    r += srcImage[i + m, j + n, 2]
                    count += 1
        dstImage[i, j] = (int(b / count), int(g / count), int(r / count))
cv2.imshow('oil painting', dstImage)
# cv2.imwrite('oil painting.jpg', dstImage)
cv2.waitKey(timeout)


cv2.destroyAllWindows()