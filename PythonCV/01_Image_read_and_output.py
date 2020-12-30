import cv2

# cv2.IMREAD_COLOR: Default parameter, which loads the original graph, a.k.a 1.
# In the case of color images, the decoded images will have the channels stored in *B G R*.
# cv2.IMREAD_GRAYSCALE: loads the gray graph, a.k.a 0.
# cv2.IMREAD_UNCHANGED: loads the graph with alpha value, a.k.a -1.
# Note that the path should not have Chinese.

image = cv2.imread('test.jpg', cv2.IMREAD_COLOR)
# print(type(image)) # numpy.ndarray
# print(image.shape) # (640, 480, 3) -> (width, height, channel)
(b, g, r) = image[200, 200]
print(b, g ,r)
cv2.imshow('test', image)
cv2.waitKey(0) # microseconds

# cv.IMWRITE_JPEG_QUALITY
# For JPEG, it can be a quality from 0 to 100 (the higher is the better). Default value is 95.
# cv.IMWRITE_PNG_COMPRESSION
# For PNG, it can be the compression level from 0 to 9. A higher value means a smaller size and longer compression time. Default value is 1 (best speed setting).
# cv2.imwrite('newTest.jpg', image, [cv2.IMWRITE_JPEG_QUALITY, 95])
