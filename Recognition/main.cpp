#include <iostream>
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <AbstractHandThresholder.h>
#include <HandThresholderFactory.h>
#include <ContourComparisonHandDetector.h>

using namespace std;
using namespace cv;


int findBiggestContour(vector<vector<Point> > contours){
    int indexOfBiggestContour = -1;
    int sizeOfBiggestContour = 0;
    for (int i = 0; i < contours.size(); i++){
        if(contours[i].size() > sizeOfBiggestContour){
            sizeOfBiggestContour = contours[i].size();
            indexOfBiggestContour = i;
        }
    }
    return indexOfBiggestContour;
}



int main()
{
   CvCapture* capture;
   Mat frame, f, hsv, bw;

   HandThresholderFactory htf;

   AbstractHandThresholder* handThresholder = htf.createInstance(HAAR_HSV);
   ContourComparisonHandDetector* handDetector = new ContourComparisonHandDetector();

   handThresholder->setDebug(1);
   handDetector->setDebug(1);
   int video = 1;

   if ( video )
   {
       capture = cvCaptureFromFile("../data/benchmark1.avi");
   }
   else
   {
          //-- Read the video stream
       capture = cvCaptureFromCAM( -1 );
   //capture = cvCreateCameraCapture(1);
   //cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 640 );
   //cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 480 );
   }




    if( capture )
   {
     while( true )
     {
       f = cvQueryFrame( capture );
       flip(f,frame,1);
       //-- 3. Apply the classifier to the frame
       if( !frame.empty() )
       {
           frame = handThresholder->thresholdHand(frame);
           handDetector->detectHand(frame);

           /*







           imshow( "aaa", frame);
*/
       }
       else
       { printf(" --(!) No captured frame -- Break!"); break; }

       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
      }
   }
   return 0;
}
