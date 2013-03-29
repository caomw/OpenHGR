#include "HaarHSVHandThresholder.h"
#include <iostream>
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

HaarHSVHandThresholder::HaarHSVHandThresholder()
{
    if( !face_cascade.load( face_cascade_name ) )
        printf("--(!)Error loading\n");
}

cv::Mat HaarHSVHandThresholder::thresholdHand ( cv::Mat input )
{
    Mat hsv, bw;
    Mat frame = substractFace (input);
    cvtColor ( frame, hsv, CV_BGR2HSV);
    blur( hsv, hsv, Size(3,3) );

    inRange(hsv, Scalar(0,  50, 0), Scalar(20, 190, 255), bw);
    erode (bw, bw, cv::Mat(), cv::Point(-1,-1), 2);
    dilate (bw, bw, cv::Mat(), cv::Point(-1,-1), 1);
    blur(bw,bw,Size(3,3));

    if ( isDebug() )
        imshow("HaarHSVHandThresholder", bw);

    return bw;
}

Mat HaarHSVHandThresholder::substractFace ( Mat input )
{
      std::vector<Rect> faces;
      Mat frame_gray;

      cvtColor( input, frame_gray, CV_BGR2GRAY );

      // Détection des visages
      face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

      for( int i = 0; i < faces.size(); i++ )
      {
        // Circle
        Point center ( faces[i].x + faces[i].width / 2.0, faces[i].y + faces[i].height / 2.0);
        circle( input, center, faces[i].width / 1.6, Scalar( 0, 0, 0), -1, 8 );
      }

      return input;
}

HaarHSVHandThresholder::~HaarHSVHandThresholder()
{
    //dtor
}
