#include "HaarYCbCrHandThresholder.h"
#include <iostream>
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

//http://spottrlabs.blogspot.ca/2012/01/super-simple-skin-detector-in-opencv.html
HaarYCbCrHandThresholder::HaarYCbCrHandThresholder()
{
    if( !face_cascade.load( face_cascade_name ) )
    printf("--(!)Error loading\n");
}

cv::Mat HaarYCbCrHandThresholder::thresholdHand ( cv::Mat input )
{
    Mat bw, bw2, ycbcr;
    cvtColor(substractFace(input), ycbcr, CV_BGR2YCrCb, 0);
    inRange(ycbcr, Scalar(0,133,77), Scalar(255,173,127), bw);

    //morphologyEx( bw, bw2, MORPH_CLOSE, element );
    //bw = bw2;

    if ( isDebug())
        imshow ( "HaarYCbCrHandThresholder", bw );

    return bw;
}

Mat HaarYCbCrHandThresholder::substractFace ( Mat input )
{
    std::vector<Rect> faces;
    Mat output = input.clone();

    Mat frame_gray;

    cvtColor( input, frame_gray, CV_BGR2GRAY );

    // Face detection
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    // Face substraction
    for( int i = 0; i < faces.size(); i++ )
    {
        // Circle
        Point center ( faces[i].x + faces[i].width / 2.0, faces[i].y + faces[i].height / 2.0);
        circle( output, center, faces[i].width / 1.4, Scalar( 0, 0, 0), -1, 8 );
    }

    return output;
}


HaarYCbCrHandThresholder::~HaarYCbCrHandThresholder()
{
    //dtor
}
