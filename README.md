# Bat Yam-1105-TouchIt

Hand recognition application that uses hand gestures as a mouse.
Written with c++ using WINAPI and opencv library.

## How the application works:
1) Run the program  
2) Place your hand so it cover the marked rectangle in the window opened. <br />
then press any key on your keyboard. <br />
3) Stabilize your background and again click on any button on your keyboard. <br />
4) Raise one finger and move the hand to move the cursor or raise two fingers to perform a click. <br />

## Algorithms used: <br />
1)Get the range of colors of the hand when the user fully covers the rectangle with his hand. <br />
2)When the user confirm the background, we save the last 10 frames and compare each frame to those frames, then we use compare the color differance to track the hand. <br />
3)When we have the difference between the frames we use the color range and perform binarization to mark in white only the hand and the face(mostly they are the same color). <br />
4)Perform edge detection over the objects. <br />
5)Perform shape check to distinguish between the face and the hand. <br />
6)perform "section iteration" to check the number of fingers raised. <br />
7)Perform the selected action using WINAPI. <br />

## requirements:
1)web cam connected to your pc.
2)windows os.

## Assumptions:
1) The backround is still.
2) The hand is bigger then the face.

## GIT Guide
```
cd existing_repo
git remote add origin https://gitlab.com/TpOiMvEoR/bat-yam-1105-touchit.git
git branch -M main
git push -uf origin main
```


