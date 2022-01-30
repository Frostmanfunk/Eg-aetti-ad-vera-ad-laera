import os
import cv2
import numpy as np
import RPi.GPIO as GPIO
#import time



#stikar fyrir myndavél
frameWidth = 640
frameHeight = 480
cap = cv2.VideoCapture(0) #finna rétt gildi fyrir myndavél
cap.set(3,frameWidth)
cap.set(4, frameHeight)

# Configuration for GPIO pins
RELAY_FAN_GPIO_PIN = 4 # BCM pin used to turn RELAY for FAN ON/OFF
GPIO.setwarnings(False)
  GPIO.setmode(GPIO.BCM)
  # Setting up relay for FAN
  GPIO.setup(RELAY_FAN_GPIO_PIN, GPIO.OUT, initial=GPIO.LOW) # HIGH MEANS LIGHT IS ON

#til að greina umferðarljós er eftirfarandi kóði notaður
verify_g = 0
verify_r = 0
try:
    while True:
        font = cv2.FONT_HERSHEY_SIMPLEX
        _, img = cap.read()
        cimg = img
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

        # color range
        lower_red1 = np.array([0,0,255])
        upper_red1 = np.array([82,255,255])
        lower_red2 = np.array([160,100,100])
        upper_red2 = np.array([180,255,255])
        lower_green = np.array([72,0,0]) #var [40;50;50]
        upper_green = np.array([103,101,255])
        # lower_yellow = np.array([15,100,100])
        # upper_yellow = np.array([35,255,255])
        lower_yellow = np.array([15,150,150])
        upper_yellow = np.array([35,255,255])
        mask1 = cv2.inRange(hsv, lower_red1, upper_red1)
        mask2 = cv2.inRange(hsv, lower_red2, upper_red2)
        maskg = cv2.inRange(hsv, lower_green, upper_green)
        masky = cv2.inRange(hsv, lower_yellow, upper_yellow)
        maskr = cv2.add(mask1, mask2)

        size = img.shape
        # print size

        # hough circle detect
        r_circles = cv2.HoughCircles(maskr, cv2.HOUGH_GRADIENT, 1, 80,
                                param1=50, param2=10, minRadius=5, maxRadius=30)

        g_circles = cv2.HoughCircles(maskg, cv2.HOUGH_GRADIENT, 1, 60,
                                    param1=50, param2=10, minRadius=5, maxRadius=30)

        y_circles = cv2.HoughCircles(masky, cv2.HOUGH_GRADIENT, 1, 30,
                                    param1=50, param2=5, minRadius=5, maxRadius=30)

        # traffic light detect
        r = 5
        
        bound = 4.0 / 10
        if r_circles is not None:
            r_circles = np.uint16(np.around(r_circles))

            for i in r_circles[0, :]:
                if i[0] > size[1] or i[1] > size[0]or i[1] > size[0]*bound:
                    continue

                h, s = 0.0, 0.0
                for m in range(-r, r):
                    for n in range(-r, r):

                        if (i[1]+m) >= size[0] or (i[0]+n) >= size[1]:
                            continue
                        h += maskr[i[1]+m, i[0]+n]
                        s += 1
                if h / s > 50:
                    cv2.circle(cimg, (i[0], i[1]), i[2]+10, (0, 255, 0), 2)
                    cv2.circle(maskr, (i[0], i[1]), i[2]+30, (255, 255, 255), 2)
                    cv2.putText(cimg,'RED',(i[0], i[1]), font, 1,(255,0,0),2,cv2.LINE_AA)
                    #print("Rautt ljós")

        if g_circles is not None:
            g_circles = np.uint16(np.around(g_circles))

            for i in g_circles[0, :]:
                if i[0] > size[1] or i[1] > size[0] or i[1] > size[0]*bound:
                    continue

                h, s = 0.0, 0.0
                for m in range(-r, r):
                    for n in range(-r, r):

                        if (i[1]+m) >= size[0] or (i[0]+n) >= size[1]:
                            continue
                        h += maskg[i[1]+m, i[0]+n]
                        s += 1
                if h / s > 100:
                    cv2.circle(cimg, (i[0], i[1]), i[2]+10, (0, 255, 0), 2)
                    cv2.circle(maskg, (i[0], i[1]), i[2]+30, (255, 255, 255), 2)
                    cv2.putText(cimg,'GREEN',(i[0], i[1]), font, 1,(255,0,0),2,cv2.LINE_AA)
                    verify_g +=1
                    #print(verify_g)
                    #print("Grænt ljós")

        if y_circles is not None:
            y_circles = np.uint16(np.around(y_circles))

            for i in y_circles[0, :]:
                if i[0] > size[1] or i[1] > size[0] or i[1] > size[0]*bound:
                    continue

                h, s = 0.0, 0.0
                for m in range(-r, r):
                    for n in range(-r, r):

                        if (i[1]+m) >= size[0] or (i[0]+n) >= size[1]:
                            continue
                        h += masky[i[1]+m, i[0]+n]
                        s += 1
                if h / s > 50:
                    cv2.circle(cimg, (i[0], i[1]), i[2]+10, (0, 255, 0), 2)
                    cv2.circle(masky, (i[0], i[1]), i[2]+30, (255, 255, 255), 2)
                    cv2.putText(cimg,'YELLOW',(i[0], i[1]), font, 1,(255,0,0),2,cv2.LINE_AA)

        if verify_g >=20 and r_circles is None:
            verify_g = 0 #Hér mun koma kóði sem lætur robotinn fara af stað
            GPIO.output(RELAY_FAN_GPIO_PIN,GPIO.HIGH) # Turn the signal on
            print("Gó")

        #cv2.imshow('detected results', cimg)
        # cv2.imshow('maskr', maskr)
        # cv2.imshow('maskg', maskg)
        # cv2.imshow('masky', masky)
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

    cap.release()
    cv2.destroyAllWindows()
    
# trap a CTRL+C keyboard interrupt
except KeyboardInterrupt:
  GPIO.cleanup() # resets all GPIO ports used by this function
