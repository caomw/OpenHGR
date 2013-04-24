#include "HaarFaceSubstractor.h"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

HaarFaceSubstractor::HaarFaceSubstractor()
{
    if( !face_cascade.load( face_cascade_name ) )
        printf("--(!)Error loading\n");
}

HaarFaceSubstractor::~HaarFaceSubstractor()
{
    //dtor
}

Mat HaarFaceSubstractor::substractFace ( Mat input, Mat thresholdFrame )
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
        circle( thresholdFrame, center, faces[i].width / 1.3, Scalar( 0, 0, 0), -1, 8 );
    }

    if ( isDebug() )
        imshow("HaarFaceSubstractor", thresholdFrame);

    return thresholdFrame;
}
