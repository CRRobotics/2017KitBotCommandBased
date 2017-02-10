
# import the necessary packages

#from threading import Thread

#from imutils.video import WebcamVideoStream


import io
import cv2
import numpy as np
import os
import pickle
from networktables import NetworkTable
from time import sleep
from pythonSB import *
import time
import math

from threading import Thread
import cv2

###########################################################################
# This class starts a thread that continuously reads a frame from a camera
# and makes it available to the main program
###########################################################################
class WebcamVideoStream:
	def __init__(self, src=0):
		# initialize the video camera stream and read the first frame
		# from the stream
		self.stream = cv2.VideoCapture(src)
		(self.grabbed, self.frame) = self.stream.read()

		# initialize the variable used to indicate if the thread should
		# be stopped
		self.stopped = False

	def start(self):
		# start the thread to read frames from the video stream
		t = Thread(target=self.update, args=())
		t.daemon = True
		t.start()
		return self

	def update(self):
		# keep looping infinitely until the thread is stopped
		while True:
			# if the thread indicator variable is set, stop the thread
			if self.stopped:
				return

			# otherwise, read the next frame from the stream
			(self.grabbed, self.frame) = self.stream.read()

	def read(self):
		# return the frame most recently read
		return self.frame

	def stop(self):
		# indicate that the thread should be stopped
		self.stopped = True
		
##########################################################################
# This function is called whenever the user changes the trackbars.
# It grabs the current trackbar settings and places them into FilterValues
# It stores the values to saveFilter.p using pickle
##########################################################################
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
    
#####################################################################3
def greaterArea(a, b):
    if cv2.contourArea(a) > cv2.contourArea(b):
        return -1
    return 1
   
#####################################################################3
def ExitCallback():
    exit
    
#######################################
# This is the start of the main program
#######################################
cam = WebcamVideoStream(src=0).start()
#cam2 = WebcamVideoStream(src=1).start()


img=cam.read() # Throw away the first frame
while img is None:
    img = cam.read()
    time.sleep(0.2)

# Create the two named windows
cv2.namedWindow("Tracker",cv2.WINDOW_AUTOSIZE)
cv2.namedWindow("camera",cv2.WINDOW_AUTOSIZE)
#cv2.namedWindow("camera2",cv2.WINDOW_AUTOSIZE)

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
os.system('sudo v4l2-ctl -d=2 -c exposure_auto=1 -c exposure_absolute='+str(FilterValues["EXPOS"]))

# set up the network table as a server
#sd = NetworkTable.getTable("CameraTracker")
# set up the network table as a server
NetworkTable.initialize(server="roborio-639-FRC.local")
#NetworkTable.setIPAddress("roborio-639-FRC.local")
sd = NetworkTable.getTable("CameraTracker")	


XAngleToTarget = 45

TargetTime = time.time()
start_timer = time.time()
# The main loop.  Pressing the esc key will exit the loop and stop the program
while(1):
   # print time.time() - start_timer

    start_timer = time.time()
     
    img=cam.read() # Grab an image
    #print(img)
    #img2 = cam2.read()
#    cv2.imshow("camera2",img2)

#        self.rgb, self.frame = f, cv2.cvtColor(f,cv2.COLOR_BGR2HSV) # Convet it to HSV format

    HSV = cv2.cvtColor(img,cv2.COLOR_BGR2HSV) # Convert it to HSV format
    # Filter the HSV image to keep only the color range specified by the Filter values.
    mask = cv2.inRange(HSV, np.array([FilterValues["H_MIN"],FilterValues["S_MIN"],FilterValues["V_MIN"]]), np.array([FilterValues["H_MAX"],FilterValues["S_MAX"],FilterValues["V_MAX"]]))
    mask = cv2.erode(mask, None, iterations=2) # These two functions get rid of noise
    mask = cv2.dilate(mask, None, iterations=2)
    
    # time to here ~.05 second
    
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

    if len(cnts) > 0:
        # Add all the contours to the camera image
        # This helps the user to see the results of the filter
        cv2.drawContours(img,cnts, -1, (255,0,0))
        

    # only proceed if at least two contours were found
    if len(cnts) > 1:

        # find the largest contour in the mask, then use
        # it to compute the minimum enclosing circle and
        # centroid
#        c = max(cnts, key=cv2.contourArea)
        cnts.sort(key=cv2.contourArea)
   
        ((x1, y1), radius1) = cv2.minEnclosingCircle(cnts[0])
        M = cv2.moments(cnts[0])
        center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

        ((x2, y2), radius2) = cv2.minEnclosingCircle(cnts[1])
        M2 = cv2.moments(cnts[1])
        center2 = (int(M2["m10"] / M2["m00"]), int(M2["m01"] / M2["m00"]))
 
        # only proceed if the radius meets a minimum size. radius2 is the smaller of the two.
        if radius2 > 6:
            # Find the target
            x= (x1+x2)/2
            y= (y1+y2)/2
            # draw the circle and centroid on the frame,
            # then update the list of tracked points
            cv2.circle(img, (int(x1), int(y1)), int(radius1),
                (0, 255, 255), 1)
            cv2.circle(img, center, 5, (0, 0, 255), -1)

            cv2.circle(img, (int(x2), int(y2)), int(radius2),
                (0, 255, 255), 1)
            cv2.circle(img, center2, 5, (0, 0, 255), -1)
            cv2.circle(img, (int(x), int(y)), 5, (0, 255, 255), -1)

            # Find the bounding rectangle that best fits the target
            # This is a rotated rectangle so we can calculate the target angle
##            rect1 = cv2.minAreaRect(cnts[0])
##            box1 = cv2.boxPoints(rect1)
##            box1 = np.int0(box1)
##            rotation = round(np.arctan2(box1[0,1]-box1[1,1], box1[0,0]-box1[1,0])*180/np.pi,1)
##            cv2.drawContours(img, [box1],0,(0,0,255),1) # Draw the rotated box on the image
            
            # Calculate the angle to target g
            XAngleToTarget = round((x-320) * 0.095, 1)
            
            # Limit the max distance to < 15 feet (And avoid a divide by zero)
            DistanceAngleTan = math.tan(((abs(x2-x1)/2) * 0.095)/57.29)
            if DistanceAngleTan > 0.0229167: 
                Distance = 4.125/DistanceAngleTan
            else:
                Distance = 180
        
            #makes the angle come from the center of the robot, 16 is distamce from
            #the camera to the cemter of rotation
            XAngleToTarget = 57.29 * math.atan(Distance * math.sin(XAngleToTarget / 57.29) / (16 + Distance*math.cos(XAngleToTarget / 57.29)))
            imageText = "X:"+str(int(x))+" Y:"+str(int(y)) +" Angle:"+str(XAngleToTarget)+" Distance:"+str(Distance)
            cv2.putText(img,imageText,(5,470), cv2.FONT_HERSHEY_SIMPLEX, 1, (255,255,255), 1, cv2.LINE_AA)

            # Write the target location to the network table
            sd.putNumber('TargetX', int(x))
            sd.putNumber('TargetY', int(y))
            sd.putNumber('XAngleToTarget', XAngleToTarget)
            sd.putNumber('Distance', Distance)

    else:
        avgCount = 0
        avgX = 0
    if cameraWindowSource == 1:
        cv2.imshow("camera",img)




#############################
# Do it again with Camera2
##
##
##    HSV = cv2.cvtColor(img2,cv2.COLOR_BGR2HSV) # Convert it to HSV format
##    # Filter the HSV image to keep only the color range specified by the Filter values.
##    mask = cv2.inRange(HSV, np.array([FilterValues["H_MIN"],FilterValues["S_MIN"],FilterValues["V_MIN"]]), np.array([FilterValues["H_MAX"],FilterValues["S_MAX"],FilterValues["V_MAX"]]))
##    mask = cv2.erode(mask, None, iterations=2) # These two functions get rid of noise
##    mask = cv2.dilate(mask, None, iterations=2)
##    
##    # time to here ~.05 second
##    
##    if cameraWindowSource == 2:
##        cv2.imshow("camera2",mask) # Display the filter results
##
##    # find contours in the mask and initialize the current
##    # (x, y) center of the object
##    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
##        cv2.CHAIN_APPROX_SIMPLE)[-2]
##    center = None
##
##    # Draw the center line
###foo    cv2.line(img2,(320,1), (320,479), (0,0,0), 1)
##    # for 320x240 cv2.line(img,(160,1), (160,240), (0,0,0), 2)
##
##       
##    # Draw the instructional text
###foo    cv2.putText(img2,"press f for filter, n for normal, Esc to quit",(20,20), cv2.FONT_HERSHEY_SIMPLEX, .5, (255,255,255), 1, cv2.LINE_AA)
##
###foo    if len(cnts) > 0:
##        # Add all the contours to the camera image
##        # This helps the user to see the results of the filter
###foo        cv2.drawContours(img2,cnts, -1, (255,0,0))
##        
##
##    # only proceed if at least two contours were found
##    if len(cnts) > 1:
##
##        # find the largest contour in the mask, then use
##        # it to compute the minimum enclosing circle and
##        # centroid
###        c = max(cnts, key=cv2.contourArea)
##        cnts.sort(greaterArea)
##   
##        ((x1, y1), radius1) = cv2.minEnclosingCircle(cnts[0])
##        M = cv2.moments(cnts[0])
##        center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
##
##        ((x2, y2), radius2) = cv2.minEnclosingCircle(cnts[1])
##        M2 = cv2.moments(cnts[1])
##        center2 = (int(M2["m10"] / M2["m00"]), int(M2["m01"] / M2["m00"]))
## 
##        # only proceed if the radius meets a minimum size. radius2 is the smaller of the two.
##        if radius2 > 8:
##            # Find the target
##            x= (x1+x2)/2
##            y= (y1+y2)/2
##            # draw the circle and centroid on the frame,
##            # then update the list of tracked points
###foo            cv2.circle(img2, (int(x1), int(y1)), int(radius1),
## #foo               (0, 255, 255), 1)
###foo            cv2.circle(img2, center, 5, (0, 0, 255), -1)
##
###foo            cv2.circle(img2, (int(x2), int(y2)), int(radius2),
###foo                (0, 255, 255), 1)
## #foo           cv2.circle(img2, center2, 5, (0, 0, 255), -1)
## #foo           cv2.circle(img2, (int(x), int(y)), 5, (0, 255, 255), -1)
##
##            # Find the bounding rectangle that best fits the target
##            # This is a rotated rectangle so we can calculate the target angle
####            rect1 = cv2.minAreaRect(cnts[0])
####            box1 = cv2.boxPoints(rect1)
####            box1 = np.int0(box1)
####            rotation = round(np.arctan2(box1[0,1]-box1[1,1], box1[0,0]-box1[1,0])*180/np.pi,1)
####            cv2.drawContours(img, [box1],0,(0,0,255),1) # Draw the rotated box on the image
##            
##            # Calculate the angle to target g
##            XAngleToTarget = round((x-320) * 0.095, 1)
##
##            # Limit the max distance to < 15 feet (And avoid a divide by zero)
##            DistanceAngleTan = math.tan(((abs(x2-x1)/2) * 0.095)/57.29)
##            if DistanceAngleTan > 0.0229167: 
##                Distance = 4.125/DistanceAngleTan
##            else:
##                Distance = 180
##
###foo            imageText = "X:"+str(int(x))+" Y:"+str(int(y)) +" Angle:"+str(XAngleToTarget)+" Distance:"+str(Distance)
###foo            cv2.putText(img2,imageText,(5,470), cv2.FONT_HERSHEY_SIMPLEX, 1, (255,255,255), 1, cv2.LINE_AA)
##
##
##
##
###foo    if cameraWindowSource == 1:
## #foo       cv2.imshow("camera2",img2)
##
##
##
##
##
##
        
    k = cv2.waitKey(10)
        
    if k == 27:
        break # Exit the program
    
    if k == 102: # f key - display the results of the filter
        cameraWindowSource = 2
    if k == 110: # n key - display the camera image with the results added
        cameraWindowSource = 1
    if k == 103: # g key
        cameraWindowSource = 3
        
print("Exiting")
#cam.release()
cam.stop()
#cam2.stop()
# There is an issue closing the windows.
# Stackoverflow suggested the following code
for i in range(1,10):
    cv2.destroyAllWindows()
    cv2.waitKey(1)

                
