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

stop = False

while(True):

    ret, frame = cap.read()
    frame = cv2.flip(frame, 1)
    height, width, _ = frame.shape


    # width =70, height =70
    cv2.rectangle(frame, (width // 2+115, height//2-55),
                  (width // 2+185, height//2+15), (0, 255, 0),  1)
    lowerish = [179, 255, 255]
    higher = [0, 0, 0]
    imgHsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    for i in range(width // 2 + 115, width // 2 + 185):
        for j in range(height // 2 + -55, height // 2 + 15):
            curr = imgHsv[j][i]

            if curr[0] > higher[0]:
                higher[0] = curr[0]
            if curr[0] < lowerish[0]:
                lowerish[0] = curr[0]

            if curr[1] > higher[1]:
                higher[1] = curr[1]
            if curr[1] < lowerish[1]:
                lowerish[1] = curr[1]

            if curr[2] > higher[2]:
                higher[2] = curr[2]
            if curr[2] < lowerish[2]:
                lowerish[2] = curr[2]

    h_min = cv2.getTrackbarPos("HUE Min", "HSV")
    h_max = cv2.getTrackbarPos("HUE Max", "HSV")
    s_min = cv2.getTrackbarPos("SAT Min", "HSV")
    s_max = cv2.getTrackbarPos("SAT Max", "HSV")
    v_min = cv2.getTrackbarPos("VALUE Min", "HSV")
    v_max = cv2.getTrackbarPos("VALUE Max", "HSV")
    # apply gussian blur
    blur = cv2.GaussianBlur(frame, (3, 3), cv2.BORDER_DEFAULT)

    lower = np.array(lowerish)
    upper = np.array(higher)
    mask = cv2.inRange(imgHsv, lower, upper)
    result = cv2.bitwise_and(blur, blur, mask=mask)

    kernel = np.ones((5, 5), np.uint8)

    # The first parameter is the original image,
    # kernel is the matrix with which image is
    # convolved and third parameter is the number
    # of iterations, which will determine how much
    # you want to erode/dilate a given image.
    img_erosion = cv2.erode(mask, kernel, iterations=1)
    img_dilation = cv2.dilate(img_erosion, kernel, iterations=1)

    cv2.imshow('Dilation', img_dilation)
    cv2.imshow('HSV Color Space', imgHsv)
    cv2.imshow('Mask', mask)
    cv2.imshow('Result', result)

    # apply edge detection filter on the image
    identity = cv2.filter2D(src=mask, ddepth=-1, kernel=kernel)
    cv2.imshow('Identity', identity)

    # cv2.imshow('frame', frame)
    if cv2.waitKey(1) & 0xff == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
