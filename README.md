
# Bat Yam-1105-TouchIt

A hand recognition application that uses hand gestures to simulate mouse functionality.  
Developed in C++ with the WINAPI and OpenCV libraries.

## How the Application Works:
1. Launch the program.  
2. Place your hand to fully cover the marked rectangle in the displayed window.  
   Then, press any key on your keyboard.  
3. Stabilize the background and press any key again.  
4. Raise one finger to move the cursor or raise two fingers to perform a click.  

## Algorithm Overview:
1. Detect the hand's color range when the user completely covers the rectangle.  
2. After background confirmation, capture the last 10 frames and compare them to track the hand using color differences.  
3. Apply binarization to isolate the hand and face by marking them in white (assuming they are similar in color).  
4. Perform edge detection on the objects.  
5. Differentiate between the hand and face based on shape analysis.  
6. Use "section iteration" to count the number of raised fingers.  
7. Execute the corresponding action via WINAPI.  

## Requirements:
1. A webcam connected to your PC.  
2. Windows operating system.  

## Assumptions:
1. The background remains static.  
2. The hand is larger than the face.  

## Git Guide:
```bash
cd existing_repo
git remote add origin https://gitlab.com/TpOiMvEoR/bat-yam-1105-touchit.git
git branch -M main
git push -uf origin main
```
