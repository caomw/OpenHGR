#include <iostream>
#include <stdio.h>

// OpenCV includes
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// Filter/Filter factories includes
#include <AbstractHandThresholder.h>
#include <AbstractHandDetector.h>
#include <HandThresholderFactory.h>
#include <HandDetectorFactory.h>

using namespace std;
using namespace cv;

int main()
{
   CvCapture* capture;
   Mat frame, f, hsv, bw;

   HandThresholderFactory htf;
   HandDetectorFactory hdf;

   AbstractHandThresholder* handThresholder = htf.createInstance(HAAR_HSV);
   AbstractHandDetector* handDetector = hdf.createInstance(CONTOUR_COMPARISON);

   handThresholder->setDebug(1);
   handDetector->setDebug(1);
   int video = 1;

   if ( video )
   {
       capture = cvCaptureFromFile("../data/benchmark1.avi");
   }
   else
   {
       capture = cvCaptureFromCAM( -1 );
       //capture = cvCreateCameraCapture(1);
       //cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 640 );
       //cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 480 );
   }

    if ( capture )
    {
        while( true )
        {
            f = cvQueryFrame( capture );
            flip(f,frame,1);
            if( !frame.empty() )
            {
                frame = handThresholder->thresholdHand(frame);
                handDetector->detectHand(frame);
            }
            else
            {
                printf(" --(!) No captured frame -- Break!"); break;
            }

            int c = waitKey(10);
            if( (char)c == 'c' ) { break; }
        }
    }

    return 0;
}
