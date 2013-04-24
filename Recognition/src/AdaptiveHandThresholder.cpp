#include "AdaptiveHandThresholder.h"
#include <iostream>
#include <stdio.h>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"

using namespace std;
using namespace cv;

AdaptiveHandThresholder::AdaptiveHandThresholder()
{
}

cv::Mat AdaptiveHandThresholder::thresholdHand ( cv::Mat input )
{
    Mat frame, bw;
    IplImage *img = new IplImage(input);
    if ( filterMask == NULL )
        filterMask = cvCreateImage( cvSize(img->width, img->height), IPL_DEPTH_8U, 1);

    CvAdaptiveSkinDetector filter(1, CvAdaptiveSkinDetector::MORPHING_METHOD_ERODE_DILATE);
    filter.process(img, filterMask);

    Mat mat(filterMask);
    inRange(mat, Scalar(1,1,1), Scalar(255,255,255), frame);

    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    morphologyEx( frame, bw, MORPH_CLOSE, element );

    if ( isDebug() )
        imshow ( "AdaptiveHandThresholder", bw);

    return bw;
}

AdaptiveHandThresholder::~AdaptiveHandThresholder()
{
    if ( filterMask != NULL )
        cvReleaseImage ( &filterMask );
}

