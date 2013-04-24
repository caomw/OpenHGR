#include "HSVHandThresholder.h"
#include <iostream>
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

HSVHandThresholder::HSVHandThresholder()
{
}

cv::Mat HSVHandThresholder::thresholdHand ( cv::Mat input )
{
    Mat hsv, bw, bw2, bw3;

    cvtColor ( input, hsv, CV_BGR2HSV);
    //inRange(hsv, minHSV, maxHSV, bw);

    inRange(hsv, minHSV1, maxHSV1, bw2);
    inRange(hsv, minHSV2, maxHSV2, bw3);
    bitwise_or(bw2,bw3,bw);

    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    morphologyEx( bw, bw2, MORPH_CLOSE, element );
    bw = bw2;

    if ( isDebug() )
        imshow("HSVHandThresholder", bw2);

    return bw;
}

HSVHandThresholder::~HSVHandThresholder()
{
}
