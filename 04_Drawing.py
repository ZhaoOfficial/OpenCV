import cv2
import numpy as np

timeout = 1
H = 480
W = 640
C = 3
background = np.zeros((H, W, C), dtype = np.uint8)


# line
### param(dstImage, start, end, color, width)
cv2.line(background, (100, 100), (200, 300), (0, 0, 255))
cv2.line(background, (100, 100), (400, 200), (0, 255, 255), 20)
### much smoother
cv2.line(background, (400, 200), (200, 300), (0, 255, 0), 20, cv2.LINE_AA)
cv2.imshow("result", background)
cv2.waitKey(timeout)


# shape
background = np.zeros((H, W, C), dtype = np.uint8)
### param(dstImage, center, radius, color, width) width = -1: fill the shape
cv2.circle(background, (250, 250), (50), (255, 0, 255), 2)
### param(dstImage, leftup, rightdown, color, width)
cv2.rectangle(background, (50, 50), (150, 100), (255, 0, 0), 5)
### param(dstImage, center, axis, angle, start angle, end angle, color, width)
cv2.ellipse(background, (100, 250), (100, 50), 45, 0, 180, (255, 255, 255), -1)

points = np.array([[300, 300], [400, 400], [400, 450], [450, 100]])
points.reshape(-1, 1, 2)
### param(dstImage, points, is_closed, color, width, line_type)
cv2.polylines(background, [points], False, [0, 120, 255])

# text
font = cv2.FONT_ITALIC
### param(dstImage, text, coordinate, font, size, color, width, line_type)
cv2.putText(background, "Text test.", (250, 50), font, 2, (255, 120, 0), 2, cv2.LINE_AA)
cv2.imshow("result", background)
cv2.waitKey(1000)











