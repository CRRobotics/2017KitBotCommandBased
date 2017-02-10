import io
import cv2
import numpy as np
import os
import pickle
from networktables import NetworkTable
#import RPi.GPIO as GPIO
from time import sleep
#from pythonSB import *
from threading import Thread
import time

##########################################################################
# This function is called whenever the user changes the trackbars.
# It grabs the current trackbar settings and places them into FilterValues
# It stores the values to saveFilter.p using pickle
##########################################################################

class WebcamVideoStream:
    def __init__(self, src=0):
        # initialize the video camera stream and read the first frame
        # from the stream
        self.stream = cv2.VideoCapture(src)
        #(self.grabbed, self.frame) = self.stream.read()
        self.read_frame()

        # initialize the variable used to indicate if the thread should
        # be stopped
        self.stopped = False

    def start(self):
        # start the thread to read frames from the video stream
        t = Thread(target=self.update, args=())
        t.daemon = True
        t.start()
        return self
    def read_frame(self):
        _, f = self.stream.read()
        self.rgb, self.frame = f, cv2.cvtColor(f,cv2.COLOR_BGR2HSV) # Convet it to HSV format

    def update(self):
        # keep looping infinitely until the thread is stopped
        while True:
            # if the thread indicator variable is set, stop the thread
            if self.stopped:
                return

            # otherwise, read the next frame from the stream
            self.read_frame()

    def read(self):
        # return the frame most recently read
        return self.rgb, self.frame

    def stop(self):
        # indicate that the thread should be stopped
        self.stopped = True

def SaveParameters(*arg):
    # See if the exposure changed. If so, write it to the camera.
    if (FilterValues["EXPOS"] != cv2.getTrackbarPos("EXPOS", "Tracker")):
        os.system('sudo v4l2-ctl -c exposure_auto=1 -c exposure_absolute='+str(cv2.getTrackbarPos("EXPOS", "Tracker")))

    FilterValues["H_MIN"] = cv2.getTrackbarPos("H_Min", "Tracker")
    FilterValues["H_MAX"] = cv2.getTrackbarPos("H_Max", "Tracker")
    FilterValues["S_MIN"] = cv2.getTrackbarPos("S_Min", "Tracker")
    FilterValues["S_MAX"] = cv2.getTrackbarPos("S_Max", "Tracker")
    FilterValues["V_MIN"] = cv2.getTrackbarPos("V_Min", "Tracker")
    FilterValues["V_MAX"] = cv2.getTrackbarPos("V_Max", "Tracker")
    FilterValues["EXPOS"] = cv2.getTrackbarPos("EXPOS", "Tracker")
    output = open("saveFilter.p", "wb")
    pickle.dump(FilterValues,output)
    output.close
    
    
 
def ExitCallback():
    exit
    
#######################################
# This is the start of the main program
#######################################
 
cam=WebcamVideoStream(0) # Open the camera
cam.start()

##cam.set(3,320) # Set lower camera resolution for speed.
##cam.set(4,240)
img,_=cam.read() # Throw away the first frame

# Create the two named windows
cv2.namedWindow("Tracker",cv2.WINDOW_AUTOSIZE)
cv2.namedWindow("camera",cv2.WINDOW_AUTOSIZE)

# cameraSource is used to switch between different views.
# If it is 1 then show the camera image with the results of
#   of the tracking added to the image.
# If it is 2 then show the results of the filtering.
cameraWindowSource = 1


# Set up a dictionary to store the filter parameters. Assign initial values.
FilterValues = {"H_MIN":52,"H_MAX":86,"S_MIN":77,"S_MAX":255,"V_MIN":111,"V_MAX":255,"EXPOS":200}

# Try to load the previous values from a pickle file.
try:
    FilterValues = pickle.load(open("saveFilter.p", "rb"))
except IOError:
    print ("No parameter file")

cv2.createTrackbar("H_Min", "Tracker", FilterValues["H_MIN"], 255, SaveParameters)
cv2.createTrackbar("H_Max", "Tracker", FilterValues["H_MAX"], 255, SaveParameters)
cv2.createTrackbar("S_Min", "Tracker", FilterValues["S_MIN"], 255, SaveParameters)
cv2.createTrackbar("S_Max", "Tracker", FilterValues["S_MAX"], 255, SaveParameters)
cv2.createTrackbar("V_Min", "Tracker", FilterValues["V_MIN"], 255, SaveParameters)
cv2.createTrackbar("V_Max", "Tracker", FilterValues["V_MAX"], 255, SaveParameters)
cv2.createTrackbar("EXPOS", "Tracker", FilterValues["EXPOS"], 500, SaveParameters)

# This will attempt to set the exposure of the camera.
# It is not very reliable.
os.system('sudo v4l2-ctl -c exposure_auto=1 -c exposure_absolute='+str(FilterValues["EXPOS"]))

# set up the network table as a server
NetworkTable.initialize(server="roborio-639-FRC.local")
#NetworkTable.setIPAddress("roborio-639-FRC.local")
sd = NetworkTable.getTable("CameraTracker")	
XAngleToTarget = 45

TargetTime = time.time()

# The main loop.  Pressing the esc key will exit the loop and stop the program
ticks = 0
while(1):
    #print('a') 
    img,HSV=cam.read()  # Grab an image
    ftime = time.monotonic()
    #print('b')
    #print('c')
    # Filter the HSV image to keep only the color range specified by the Filter values.
    mask = cv2.inRange(HSV, np.array([FilterValues["H_MIN"],FilterValues["S_MIN"],FilterValues["V_MIN"]]), np.array([FilterValues["H_MAX"],FilterValues["S_MAX"],FilterValues["V_MAX"]]))
    mask = cv2.erode(mask, None, iterations=2) # These two functions get rid of noise
    #print('d')
    mask = cv2.dilate(mask, None, iterations=2)
    #print('e')

    if cameraWindowSource == 2:
        cv2.imshow("camera",mask) # Display the filter results

    # find contours in the mask and initialize the current
    # (x, y) center of the object
    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
         cv2.CHAIN_APPROX_SIMPLE)[-2]
    center = None

    # Draw the center line
    cv2.line(img,(320,1), (320,479), (0,0,0), 1)
    # for 320x240 cv2.line(img,(160,1), (160,240), (0,0,0), 2)

    # Draw the instructional text
    cv2.putText(img,"press f for filter, n for normal, Esc to quit",(20,20), cv2.FONT_HERSHEY_SIMPLEX, .5, (255,255,255), 1, cv2.LINE_AA)
    
    # only proceed if at least one contour was found
    if len(cnts) > 0:

        # Add all the contours to the camera image
        # This helps the user to see the results of the filter
        cv2.drawContours(img,cnts, -1, (255,0,0))
        
        # find the largest contour in the mask, then use
        # it to compute the minimum enclosing circle and
        # centroid
        c = max(cnts, key=cv2.contourArea)
        
        ((x, y), radius) = cv2.minEnclosingCircle(c)
        M = cv2.moments(c)
        center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
        print("aa")
 
        # only proceed if the radius meets a minimum size
        if radius > 10:

            # draw the circle and centroid on the frame,
            # then update the list of tracked points
            #cv2.circle(img, (int(x), int(y)), int(radius),
            #    (0, 255, 255), 1)
            #cv2.circle(img, center, 5, (0, 0, 255), -1)

            # Find the bounding rectangle that best fits the target
            # This is a rotated rectangle so we can calculate the target angle
            rect1 = cv2.minAreaRect(c)
            box1 = cv2.boxPoints(rect1)
            box1 = np.int0(box1)
            rotation = round(np.arctan2(box1[0,1]-box1[1,1], box1[0,0]-box1[1,0])*180/np.pi,1)
            cv2.drawContours(img, [box1],0,(0,0,255),1) # Draw the rotated box on the image
            
            # Calculate the angle to target 
            XAngleToTarget = round((x-320) * 0.068, 1)
            # for 320x240XAngleToTarget = (x-160) * 0.14
            # Write the target location to the network table
            sd.putNumber('TargetX', int(x))
            sd.putNumber('TargetY', int(y))
            sd.putNumber('XAngleToTarget', XAngleToTarget)
            sd.putNumber('ticker', ticks)
            ticks += 1
            imageText = "X:"+str(int(x))+" Y:"+str(int(y)) +" Angle:"+str(XAngleToTarget)+" Rotation:"+str(rotation)
            cv2.putText(img,imageText,(5,470), cv2.FONT_HERSHEY_SIMPLEX, 1, (255,255,255), 1, cv2.LINE_AA)

##            CurrentServoAngle -= (XAngleToTarget)
##            if (CurrentServoAngle < 0):
##                CurrentServoAngle = 0
##            if (CurrentServoAngle > 90):
##                CurrentServoAngle = 90
##            #print(CurrentServoAngle)    
##            SetServo(CurrentServoAngle)
##            TargetTime = time.time()
##            while (time.time() < TargetTime + .1): #wait for the servo to move
##                _,img2=cam.read()  # Grab an image to throw away.
##                                   # Gets rid of blurry shots
                
    else:
        avgCount = 0
        avgX = 0
    tt = time.monotonic()
    dt = tt - ftime
    cv2.putText(img, "{:.1f}".format(dt*1000), (560, 15), cv2.FONT_HERSHEY_SIMPLEX, .5, (0xff,0xff,0xff), 1, cv2.LINE_AA)
    if cameraWindowSource == 1:
        cv2.imshow("camera",img)
        
    k = cv2.waitKey(1)
        
    if k == 27:
        break # Exit the program
    
    if k == 102: # f key - display the results of the filter
        cameraWindowSource = 2
    if k == 110: # n key - display the camera image with the results added
        cameraWindowSource = 1
     
print("Exiting")
#cam.release()

# There is an issue closing the windows.
# Stackoverflow suggested the following code
for i in range(1,10):
    cv2.destroyAllWindows()
    cv2.waitKey(1)

                
