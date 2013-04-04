import cv2
import numpy as np
import os
import time

INPUTDIR = "/home/mathieu/Pictures/mains/"
OUTPUTDIR = "output"

def cropImage ( filename, outputFilename ):
    image = cv2.imread(filename )
    
    #Threshold
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    bw = cv2.inRange(hsv, np.array([0,50,0]), np.array([30,255,255]))
    
    element = cv2.getStructuringElement(cv2.MORPH_CROSS,(5,5))
    for i in range(0,3):
        bw2 = cv2.erode(bw, element)
        bw = cv2.dilate(bw, element)
    
    #Contour
    bw2 = cv2.GaussianBlur(bw, (5,5), 0)
    canny = cv2.Canny(bw2, 0, 300)
    
    contours, hierarchy = cv2.findContours(canny, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)   
    
    #Biggest contour
    biggestContourId = -1
    biggestContour = 0
    
    for i in range(len(contours)):
        #color = np.random.randint(0,255,(3)).tolist()
        #cv2.drawContours(image, contours, i,color,2)
        
        c = contours[i]
        x,y,w,h = cv2.boundingRect(c)
        area = w*h
        if area > biggestContour :
            biggestContour = area
            biggestContourId = i
        
    #Crop
    if biggestContourId != -1:
        x,y,w,h = cv2.boundingRect(contours[biggestContourId])
        cropped = image[y:y+h,x:x+w]
        gray = cv2.cvtColor(cropped,cv2.COLOR_BGR2GRAY)
        cv2.imwrite(outputFilename ,gray )

    #cv2.imwrite("Test/" + "aaa.jpg" ,image )

startTime = time.time()

for path, dirs, filenames in os.walk("/home/mathieu/Pictures/mains/"):
    for f in filenames:
        #cropImage ( path, f,  )
        inputFile = path + "/" + f
        outputDirectory = OUTPUTDIR + "/" + path.replace(INPUTDIR, "")
        outputFile = outputDirectory + "/" + f
        if not os.path.exists(outputDirectory): os.makedirs(outputDirectory)
        cropImage (inputFile, outputFile)

print "OVER!" + str(time.time() - startTime)



