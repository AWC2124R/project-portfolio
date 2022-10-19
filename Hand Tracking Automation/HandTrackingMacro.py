from math import dist
import cv2
import mediapipe as mp
import pyautogui
import time

# Start video capture using webcam
_videoCapture = cv2.VideoCapture(0)
_videoCapture.set(3, 1920)
_videoCapture.set(4, 1080)

# Get mediapipline for hand detection baseline software
_mediaPipeHands = mp.solutions.hands
_handsDetection = _mediaPipeHands.Hands(static_image_mode=False, max_num_hands=1, min_detection_confidence=0.3, min_tracking_confidence=0.3)
_mediaPipeDraw = mp.solutions.drawing_utils

# Variables for FPS / frame calculations
_pTime = 0
_cTime = 0

# Variables for macro
distance4w8 = 100
distanceBefore4w8 = 100
yDistance4w8 = 100
yDistanceBefore4w8 = 100

distance4w12 = 100
distance4w16 = 100
distance4w20 = 100

beforeVU = False
beforeVD = False
currentlyVU = False
currentlyVD = False

DIST_THRESHOLD = 0.075
Y_MOVEMENT_THRESHOLD = 0.05

# Loop for tracking / main loop
while True:
    success, img = _videoCapture.read()  # Get image from webcam
    rgbIMG = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)  # Change image from BGR to RGB
    hsvIMG = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    results = _handsDetection.process(rgbIMG)  # Get hands from mediapipeline

    # Check for Inputs
    
    if results.multi_hand_landmarks:
        for handLimbs in results.multi_hand_landmarks:
            landmarkPos =  [[handLimbs.landmark[i].x, handLimbs.landmark[i].y, handLimbs.landmark[i].z] for i in range(21)]
            
            distanceBefore4w8 = distance4w8
            yDistanceBefore4w8 = yDistance4w8

            yDistance4w8 = (handLimbs.landmark[4].y + handLimbs.landmark[8].y) / 2

            distance4w8 = dist(landmarkPos[4], landmarkPos[8])
            distance4w12 = dist(landmarkPos[4], landmarkPos[12])
            distance4w16 = dist(landmarkPos[4], landmarkPos[16])
            distance4w20 = dist(landmarkPos[4], landmarkPos[20])
            
            beforeVU = currentlyVU
            beforeVD = currentlyVD

            if distance4w8 < DIST_THRESHOLD:
                if Y_MOVEMENT_THRESHOLD < yDistanceBefore4w8 - yDistance4w8 or beforeVU:
                    currentlyVU = True
                if Y_MOVEMENT_THRESHOLD < yDistance4w8 - yDistanceBefore4w8 or beforeVD:
                    currentlyVD = True
            else:
                currentlyVU = False
                currentlyVD = False

            if distance4w12 < DIST_THRESHOLD:
                pyautogui.hotkey('win', 'e')
                time.sleep(.2)
            
            if distance4w16 < DIST_THRESHOLD:
                pyautogui.hotkey('win', 'tab')
                time.sleep(.2)

    else:
        currentlyVU = False
        currentlyVD = False
    
    if currentlyVU:
        pyautogui.press('volumeup')
    elif currentlyVD:
        pyautogui.press('volumedown')

    if results.multi_hand_landmarks:
        for handLimbs in results.multi_hand_landmarks:  # Enumerate between hands

            for id, limb in enumerate(handLimbs.landmark):  # Enumerate between nodes on hands
                height, width, x = img.shape
                convertX, convertY = int(limb.x * width), int(limb.y * height)  # Frame size ratio to pixel placement
                # print("ID: " + str(id) + " | POS:" + " x=" + str(convertX) + " y=" + str(convertY) + " z=" + str(int(abs(limb.z * 255 * 10))))

                # Draw circles at landmarks
                cv2.circle(img, (convertX, convertY), 10, (int(abs(limb.z * 255 * 10)), int(abs(limb.z * 255 * 10)), int(abs(limb.z * 255 * 10))), cv2.FILLED)

            # Draw lines between landmarks
            _mediaPipeDraw.draw_landmarks(img, handLimbs, _mediaPipeHands.HAND_CONNECTIONS)

    # FPS Calculation
    _cTime = time.time()
    fps = 1/(_cTime - _pTime)
    _pTime = _cTime

    # Show frame
    cv2.imshow("_videoCapture - IMG", img)
    cv2.waitKey(1)

# Portions of this code were built on code from [ https://www.section.io/engineering-education/creating-a-hand-tracking-module/ ].
