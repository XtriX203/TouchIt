import numpy as np
import cv2

cap = cv2.VideoCapture(0)
ret, frame = cap.read()

# the kernal filter for the convolution as matrix
kernel = np.array([[-1, -1, -1],
                   [-1, 8, -1],
                   [-1, -1, -1]])


while(True):
    ret, frame = cap.read()
    frame = cv2.flip(frame, 1)
    # apply gussian blur
    blur = cv2.GaussianBlur(frame, (3, 3), cv2.BORDER_DEFAULT)

    # apply edge detection filter on the image
    identity = cv2.filter2D(src=blur, ddepth=-1, kernel=kernel)
    cv2.imshow('Identity', identity)

    cv2.imshow('frame', frame)
    if cv2.waitKey(1) & 0xff == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
