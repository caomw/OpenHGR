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
    Mat hsv, bw, bw2, cannyOutput;
    Mat frame = substractFace (input);

    cvtColor ( frame, hsv, CV_BGR2HSV);
    inRange(hsv, minHSV, maxHSV, bw);
    //blur(bw,bw,Size(3,3));

    // Enlever les trous >> Marche mal
    Mat element = getStructuringElement( MORPH_RECT, Size( 2*erosionSize + 1, 2*erosionSize+1 ),Point( erosionSize,erosionSize ) );
    erode (bw, bw, element);
    dilate (bw, bw, element);


    bitwise_not(bw,bw2);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Canny( bw2, cannyOutput, 50, 50*2, 3 );

    findContours( cannyOutput, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    for( int i = 0; i< contours.size(); i++ )
        drawContours( bw, contours, i, Scalar(255), -1, 8, hierarchy, 0, Point() );

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
