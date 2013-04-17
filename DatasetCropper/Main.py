import cv2
import numpy as np
import os
import time

INPUTDIR = "/home/mathieu/Pictures/mains"
OUTPUTDIR = "output"

def getArmMask ( image ):
    gray = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)
    bw = cv2.inRange(gray, np.array([0]), np.array([20]))
    element = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(3,3))
    bw2=cv2.erode(bw,element)
    bw =cv2.dilate(bw2,element)
    bw2 =cv2.dilate(bw,element)
    return cv2.bitwise_not(bw2)

def getGreenMask (image ):
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    bw = cv2.inRange(hsv, np.array([45,0,0]), np.array([100,255,255]))
    return cv2.bitwise_not(bw)

def getMask (image):
    arm = getArmMask(image)
    green = getGreenMask(image)
    
    return cv2.bitwise_and(arm, green)

def cropImage ( filename, outputFilename ):
    image = cv2.imread(filename )
    
    #Threshold
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)       #30
    bw = cv2.inRange(hsv, np.array([0,50,0]), np.array([70,255,255]))
    
    ######################### TECHNIQUE 1
    #element = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(5,5))
    #bw = cv2.dilate(getMask(image),element)

    #for i in range(0,3):
    #    bw2 = cv2.dilate(bw,element)
    #    bw = cv2.dilate(bw2,element)
     
    ########################### TECHNIQUE 2
    element = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(5,5))
    bw2 = cv2.erode(bw,element)
    bw = cv2.erode(bw2,element)
    
    for i in range(0,5):
        bw2 = cv2.dilate(bw,element)
        bw = cv2.dilate(bw2,element)
    
    
        
    bw2 = cv2.GaussianBlur(bw, (5,5), 0)
    #cv2.imwrite("aaa.jpg" ,bw2 )
    ##########################################
    canny = cv2.Canny(bw2, 0, 300)
    
    contours, hierarchy = cv2.findContours(canny, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)   
    
    #Biggest contour
    biggestContourId = -1
    biggestContour = 0
    
    contoursInt = [c.astype('int') for c in contours]
    
    for i in range(len(contours)):
        c = contours[i].astype('int')
        x,y,w,h = cv2.boundingRect(c)
        area = w*h
                        
        
        #color = np.random.randint(0,255,(3)).tolist()
        #cv2.drawContours(image, contoursInt, i,color,2)
        #cv2.rectangle(image, (x,y), (x+w, y+h), np.random.randint(0,255,(3)).tolist())
        #print str(w)+":"+str(h)   
        if area > biggestContour :
            biggestContour = area
            biggestContourId = i
            biggestRect=[x,y,w,h]
                

    #Crop
    if biggestContourId != -1:
        x,y,w,h=tuple(biggestRect)
        #if w > h:
        #    l = w
        #else:
        #    l = h
        #x = x + w - l
        #y = y + h - l    
        #cropped = image[y:y+l,x:x+l]
        #resized = cv2.resize(cropped, (100,100)) 
        #1gray = cv2.cvtColor(resized,cv2.COLOR_BGR2GRAY)
        #h += 15
        cropped = image[y:y+h, x:x+w]
        gray = cv2.cvtColor(cropped, cv2.COLOR_BGR2GRAY)
        if w > h:
            h = int(float(h)/w*100)
            w = 100
        else:
            w = int(float(w)/h*100)
            h = 100
                        
        resized = cv2.resize(gray,(w,h))
        cv2.imwrite(outputFilename ,resized )

startTime = time.time()

for path, dirs, filenames in os.walk(INPUTDIR):
    for f in filenames:
        inputFile = path + "/" + f
        outputDirectory = OUTPUTDIR + "/" + path.replace(INPUTDIR, "")
        outputFile = outputDirectory + "/" + f
        if not os.path.exists(outputDirectory): os.makedirs(outputDirectory)
        cropImage (inputFile, outputFile)

#cropImage("/home/mathieu/Pictures/mains/Martin/Pouce/Image-1.jpg","test.jpg")

#print "OVER!" + str(time.time() - startTime)



