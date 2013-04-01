#include "HaarLumaHandThresholder.h"
#include <iostream>
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

HaarLumaHandThresholder::HaarLumaHandThresholder()
{
    if( !face_cascade.load( face_cascade_name ) )
    printf("--(!)Error loading\n");
}

cv::Mat HaarLumaHandThresholder::thresholdHand ( cv::Mat input )
{
    Mat bw, bw2, ycbcr;
    cvtColor(substractFace(input), ycbcr, CV_BGR2YCrCb, 0);
    inRange(ycbcr, Scalar(128,0,0), Scalar(255,255,255), bw);

    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    morphologyEx( bw, bw2, MORPH_CLOSE, element );
    bw = bw2;

    if ( isDebug())
        imshow ( "HaarLumaHandThresholder", bw );

    return bw;
}

Mat HaarLumaHandThresholder::substractFace ( Mat input )
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


HaarLumaHandThresholder::~HaarLumaHandThresholder()
{
    //dtor
}