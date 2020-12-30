import cv2
import numpy as np

srcVideo = cv2.VideoCapture("face.mp4")

fps = srcVideo.get(cv2.CAP_PROP_FPS)
H = srcVideo.get(cv2.CAP_PROP_FRAME_HEIGHT)
W = srcVideo.get(cv2.CAP_PROP_FRAME_WIDTH)

