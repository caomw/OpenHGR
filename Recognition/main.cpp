#include <iostream>
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

String face_cascade_name = "haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;

Mat substractFace ( Mat in )
{
  std::vector<Rect> faces;
  Mat frame_gray, out;

  out = in;

  cvtColor( in, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  // Détection des visages
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  for( int i = 0; i < faces.size(); i++ )
  {
    // Rectangle
    Point center ( faces[i].x + faces[i].width / 2.0, faces[i].y + faces[i].height / 2.0);
    circle( out, center, faces[i].width / 1.6, Scalar( 0, 0, 0), -1, 8 );
  }

  return out;
}

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

 //-- 1. Load the cascades
   if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

   //-- Read the video stream
   capture = cvCaptureFromCAM( -1 );
   if( capture )
   {
     while( true )
     {
         frame = cvQueryFrame( capture );

       //-- 3. Apply the classifier to the frame
       if( !frame.empty() )
       {
           f = frame.clone ( );
           frame = substractFace(frame);
           cvtColor ( frame, hsv, CV_BGR2HSV);
           inRange(hsv, Scalar(0,  10, 60), Scalar(20, 150, 255), bw);


           Mat canny_output;
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;

            findContours( bw, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
            int s = findBiggestContour(contours);

            Mat drawing = Mat::zeros( frame.size(), CV_8UC1 );
            drawContours( frame, contours, s, Scalar(255), -1, 8, hierarchy, 0, Point() );




           imshow( "aaa", frame );

       }
       else
       { printf(" --(!) No captured frame -- Break!"); break; }

       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
      }
   }
   return 0;
}
