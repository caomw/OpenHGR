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

    // Debugging window / trackbars
    namedWindow( "HaarHSVHandThresholder", CV_WINDOW_AUTOSIZE );
    createTrackbar( "Element:\n 0: Rect - 1: Cross - 2: Ellipse", "HaarHSVHandThresholder", &morph_elem, 2, NULL );
    createTrackbar( "Kernel size:\n 2n +1", "HaarHSVHandThresholder", &morph_size, 21, NULL );
}

cv::Mat HaarHSVHandThresholder::thresholdHand ( cv::Mat input )
{
    Mat hsv, bw, bw2;
    Mat frame = substractFace (input);

    cvtColor ( frame, hsv, CV_BGR2HSV);
    inRange(hsv, minHSV, maxHSV, bw);

    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    morphologyEx( bw, bw2, MORPH_CLOSE, element );
    bw = bw2;

    if ( isDebug() )
        imshow("HaarHSVHandThresholder", bw);

    return bw;
}

Mat HaarHSVHandThresholder::substractFace ( Mat input )
{
    std::vector<Rect> faces;
    Mat frame_gray;

    cvtColor( input, frame_gray, CV_BGR2GRAY );

    // Face detection
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    // Face substraction
    for( int i = 0; i < faces.size(); i++ )
    {
        // Circle
        Point center ( faces[i].x + faces[i].width / 2.0, faces[i].y + faces[i].height / 2.0);
        circle( input, center, faces[i].width / 1.4, Scalar( 0, 0, 0), -1, 8 );
    }

    return input;
}

HaarHSVHandThresholder::~HaarHSVHandThresholder()
{
}
