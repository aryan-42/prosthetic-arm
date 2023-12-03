import serial
import time
import cv2
from cvzone.HandTrackingModule import HandDetector

cap = cv2.VideoCapture(0)
detector = HandDetector(detectionCon=0.8, maxHands=2)

def sendToArduino(sendStr):
    ser.write(sendStr.encode('utf-8'))  # change for Python3

def recvFromArduino():
    global startMarker, endMarker

    ck = ""
    x = "z"  # any value that is not an end- or startMarker
    byteCount = -1  # to allow for the fact that the last increment will be one too many

    # wait for the start character
    while ord(x) != startMarker:
        x = ser.read()

    # save data until the end marker is found
    while ord(x) != endMarker:
        if ord(x) != startMarker:
            ck = ck + x.decode("utf-8")  # change for Python3
            byteCount += 1
        x = ser.read()

    return (ck)

serPort = "COM9"
baudRate = 9600
ser = serial.Serial(serPort, baudRate)
print("Serial port " + serPort + " opened  Baudrate " + str(baudRate))

startMarker = 60
endMarker = 62

def array_to_string(input_array):
    # Use the map function to convert each element to a string
    string_array = map(str, input_array)

    # Join the string representations of the elements into a single string
    result_string = ''.join(string_array)

    return result_string

while True:
    success, img = cap.read()
    hands, img = detector.findHands(img)  # With Draw

    if hands:
        # Hand 1
        hand1 = hands[0]
        lmList1 = hand1["lmList"]  # List of 21 Landmarks points
        bbox1 = hand1["bbox"]  # Bounding Box info x,y,w,h
        centerPoint1 = hand1["center"]  # center of the hand cx,cy
        handType1 = hand1["type"]  # Hand Type Left or Right

        fingers1 = detector.fingersUp(hand1)
        if lmList1:
            fingers = detector.fingersUp(hand1)
            if fingers == [1, 1, 1, 1, 1]:
                sendToArduino('O')
            if fingers == [0, 0, 0, 0, 0]:
                sendToArduino('C')


        if len(hands) == 2:
            hand2 = hands[1]
            lmList2 = hand2["lmList"]  # List of 21 Landmarks points
            bbox2 = hand2["bbox"]  # Bounding Box info x,y,w,h
            centerPoint2 = hand2["center"]  # center of the hand cx,cy
            handType2 = hand2["type"]  # Hand Type Left or Right

            fingers2 = detector.fingersUp(hand2)
            length, info, img = detector.findDistance(centerPoint1, centerPoint2, img)  # with draw

    cv2.imshow("Image", img)
    cv2.waitKey(1)
ser.close