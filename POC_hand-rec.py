import numpy as np
import cv2
from matplotlib import pyplot as plt

cap = cv2.VideoCapture(0)

# the kernal filter for the convolution as matrix
kernel = np.array([[-1, -1, -1],
                   [-1, 8, -1],
                   [-1, -1, -1]])


def empty(a):
    pass


cv2.namedWindow("HSV")
cv2.resizeWindow("HSV", 640, 240)
cv2.createTrackbar("HUE Min", "HSV", 0, 179, empty)
cv2.createTrackbar("HUE Max", "HSV", 179, 179, empty)
cv2.createTrackbar("SAT Min", "HSV", 0, 255, empty)
cv2.createTrackbar("SAT Max", "HSV", 255, 255, empty)
cv2.createTrackbar("VALUE Min", "HSV", 0, 255, empty)
cv2.createTrackbar("VALUE Max", "HSV", 255, 255, empty)


ret, frame = cap.read()
frame = cv2.flip(frame, 1)
imgHsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
while(True):


    h_min = cv2.getTrackbarPos("HUE Min", "HSV")
    h_max = cv2.getTrackbarPos("HUE Max", "HSV")
    s_min = cv2.getTrackbarPos("SAT Min", "HSV")
    s_max = cv2.getTrackbarPos("SAT Max", "HSV")
    v_min = cv2.getTrackbarPos("VALUE Min", "HSV")
    v_max = cv2.getTrackbarPos("VALUE Max", "HSV")
    # apply gussian blur
    blur = cv2.GaussianBlur(frame, (1, 1), cv2.BORDER_DEFAULT)

    # cv2.imshow('video bw', blur)

    lower = np.array([h_min, s_min, v_min])
    upper = np.array([h_max, s_max, v_max])
    mask = cv2.inRange(imgHsv, lower, upper)
    result = cv2.bitwise_and(blur, blur, mask=mask)

    cv2.imshow('HSV Color Space', imgHsv)
    cv2.imshow('Mask', mask)
    cv2.imshow('Result', result)

    # apply edge detection filter on the image
    identity = cv2.filter2D(src=result, ddepth=-1, kernel=kernel)
    cv2.imshow('Identity', identity)

    # cv2.imshow('frame', frame)
    if cv2.waitKey(1) & 0xff == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
