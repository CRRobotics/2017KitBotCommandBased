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

# Final version of the camera tracker 

#Class to get the camera and make a video 
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
		
#save the slider values to the camera 
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
    
#exit the program
def ExitCallback():
    exit

#returns the area of the larger contour
def greaterArea(a, b):
    if cv2.contourArea(a) > cv2.contourArea(b):
        return -1
    return 1

	
#Get the main camera 
cam = WebcamVideoStream(src=0).start()

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

#Sets up the network table 
NetworkTable.initialize(server="roborio-639-FRC.local")
networkTable = NetworkTable.getTable("CameraTracker")	

XAngleToTarget = 45

TargetTime = time.time()
start_timer = time.time()

while(1):

    start_timer = time.time()
    networkTable.putNumber('time stamp', time.time());
	networkTable.putNumber('relative time stamp', time.time() - start_timer); 
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
    







