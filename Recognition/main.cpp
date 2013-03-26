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
    // Circle
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
           frame = substractFace(frame);
           cvtColor ( frame, hsv, CV_BGR2HSV);
           blur( hsv, hsv, Size(3,3) );


           inRange(hsv, Scalar(0,  50, 0), Scalar(20, 190, 255), bw);
           erode (bw, bw, cv::Mat(), cv::Point(-1,-1), 2);
           dilate (bw, bw, cv::Mat(), cv::Point(-1,-1), 1);
            blur(bw,bw,Size(3,3));//cvSmooth (bw,bw, CV_MEDIAN, 3, 1);
            imshow("Threshold", bw);



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

       }
       else
       { printf(" --(!) No captured frame -- Break!"); break; }

       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
      }
   }
   return 0;
}
