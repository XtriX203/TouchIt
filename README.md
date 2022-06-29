# Bat Yam-1105-TouchIt

hand recognition application that uses hand gestures as a mouse
written with c++ using WINAPI and opencv library.

How the application works:  
1)place your hand so it cover the marked rectangle in the window opened. <br />
then press and key on your keyboard. <br />
2)stabilize your background and again click on any button on your keyboard. <br />
3)raise one finger and move the hand to move the cursor or raise two fingers to perform a click. <br />

Algorithms used: <br />
1)when the user cover the rectangle and press the keyboard button, the program take the range of colors in the rectangle to match the hand color. <br />
2)when the user confirm the background, we take the last 10  frames and compare each frame to those frames, then we use the difference to track the hand. <br />
3) when we have the difference between the frames we use the color range and perform binarization to mark in white only the hand and the face(mostly they are the same color). <br />
4)we perform edge detection over the objects. <br />
5)we perform shape check to distinguish between the face and the hand. <br />
6)perform "section iteration" to check the number of fingers raised. <br />
7)make the action selected using WINAPI. <br />

## requirements:
1)web cam connected to your pc.
2)windows os


## GIT Guide
```
cd existing_repo
git remote add origin https://gitlab.com/TpOiMvEoR/bat-yam-1105-touchit.git
git branch -M main
git push -uf origin main
```


