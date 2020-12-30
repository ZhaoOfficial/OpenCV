import cv2
import numpy as np

timeout = 500

srcImage = cv2.imread("D:/Mywork/OpenCV/test.jpg", cv2.IMREAD_COLOR)
H, W, C = srcImage.shape
print(srcImage.shape)
cv2.imshow('origin', srcImage)
cv2.waitKey(timeout)

# scaling
## shrink
newH = int(H * 0.5)
newW = int(W * 0.5)
newImage = cv2.resize(srcImage, (newW, newH))
print(newImage.shape)
cv2.imshow('shrink', newImage)
cv2.waitKey(timeout)

## enlarge
newH = int(H * 2)
newW = int(W * 2)
newImage = cv2.resize(srcImage, (newW, newH))
print(newImage.shape)
cv2.imshow('enlarge', newImage)
cv2.waitKey(timeout)


### Nearest neighbor interpolation
### src -> dst
### src_x = int(dst_x * (src_H / dst_H))
newH = int(H / 2)
newW = int(W / 2)
dstImage = np.zeros((newH, newW, 3), dtype = np.uint8)
for newI in range(newH):
    for newJ in range(newW):
        i = int(newI * H / newH)
        j = int(newJ * W / newW)
        dstImage[newI, newJ, :] = srcImage[i, j, :]
print(dstImage.shape)
cv2.imshow('NNI', dstImage)
cv2.waitKey(timeout)

### Double linear interpolation
### If there is 4 points (0, 0), (0, 1), (1, 0) and (1, 1) then the DLI is
### f(x, y) = f(0, 0)(1 - x)(1 - y) + f(1, 0)x(1 - y) + f(0, 1)(1 - x)y + f(1, 1)xy



# cut the image
dstImage = srcImage[80:400, 80:560]
print(dstImage.shape)
cv2.imshow('cut', dstImage)
cv2.waitKey(timeout)


# translation
shiftMatrix = np.float32([[1, 0, 100], [0, 1, 200]])
dstImage = cv2.warpAffine(srcImage, shiftMatrix, (W, H))
cv2.imshow('translation', dstImage)
cv2.waitKey(timeout)

### homographic
### [[1, 0, 100], [0, 1, 200]] = [[1, 0], [0, 1]], [[100], [200]]
### [[1, 0], [0, 1]] * [x, y] + [[100], [200]] = [x + 100, y + 200]

# flipping
dstImage = np.flip(srcImage, axis = 0)
cv2.imshow('upside dowm', dstImage)
cv2.waitKey(timeout)
dstImage = np.flip(srcImage, axis = 1)
cv2.imshow('leftside right', dstImage)
cv2.waitKey(timeout)


# mirror
line = np.zeros((H, 1, 3), dtype = np.uint8) # dtype is very very important!
line[:, :, 2] = 255
mirror = np.concatenate((srcImage, line, dstImage), axis = 1)
cv2.imshow('mirror', mirror)
cv2.waitKey(timeout)


# scaling
scalingMatrix = np.float32([[0.5, 0, 0], [0, 0.5, 0]])
dstImage = cv2.warpAffine(srcImage, scalingMatrix, (W // 2, H // 2))
print(scalingMatrix)
cv2.imshow('scaling', dstImage)
cv2.waitKey(timeout)


# affine
affineSourceMatrix = np.float32([[0, 0], [0, H - 1], [W - 1, 0]])
affineDestMatrix = np.float32([[50, 50], [300, H - 200], [W - 300, 100]])
affineMatrix = cv2.getAffineTransform(affineSourceMatrix, affineDestMatrix)
dstImage = cv2.warpAffine(srcImage, affineMatrix, (W, H))
print(affineMatrix)
cv2.imshow('affine', dstImage)
cv2.waitKey(timeout)


#rotation
## cv2.getRotationMatrix2D(center, angle, scaling)
rotationMatrix = cv2.getRotationMatrix2D((W // 2, H // 2), 45, 0.5)
print(rotationMatrix)
dstImage = cv2.warpAffine(srcImage, rotationMatrix, (W, H))
cv2.imshow('rotation', dstImage)
cv2.waitKey(timeout)
 