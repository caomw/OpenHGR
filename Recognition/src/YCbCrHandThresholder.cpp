#include "YCbCrHandThresholder.h"
#include <iostream>
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

//http://spottrlabs.blogspot.ca/2012/01/super-simple-skin-detector-in-opencv.html
YCbCrHandThresholder::YCbCrHandThresholder()
{
}

cv::Mat YCbCrHandThresholder::thresholdHand ( cv::Mat input )
{
    Mat bw, bw2, ycbcr;
    cvtColor(input, ycbcr, CV_BGR2YCrCb, 0);
    inRange(ycbcr, Scalar(0,133,77), Scalar(255,173,127), bw);

    //Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    //morphologyEx( bw, bw2, MORPH_CLOSE, element );
    //bw = bw2;

    if ( isDebug())
        imshow ( "YCbCrHandThresholder", bw );

    return bw;
}

YCbCrHandThresholder::~YCbCrHandThresholder()
{
    //dtor
}
