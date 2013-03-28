#include <iostream>
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <AbstractHandThresholder.h>
#include <HandThresholderFactory.h>

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


   AbstractHandThresholder* handThresholder = (new HandThresholderFactory())->getHandThresholder(HAAR_HSV);
   handThresholder->setDebug(1);
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


           /*



            Mat canny_output;
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;

            Canny( bw, canny_output, 50, 50*2, 3 );
            findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
            //dint s = findBiggestContour(contours);

            imshow("Canny", canny_output);

            Mat drawing = Mat::zeros( frame.size(), CV_8UC1 );
            for( int i = 0; i< contours.size(); i++ )
            {
               Scalar color = Scalar( rand()%255, rand()%255, rand()%255 );
               drawContours( frame, contours, i, color, 2, 8, hierarchy, 0, Point() );
            }

            //for ( int s = 0; s < contours.size(); s ++ )
              //drawContours( frame, contours, s, Scalar(rand() % 255,rand() % 255,rand() % 255), -1, 8, hierarchy, 0, Point() );

              //drawContours( frame, contours, s, Scalar(255), -1, 8, hierarchy, 0, Point() );



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
