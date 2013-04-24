#include "YCbCrEllipseHandThresholder.h"
#include <iostream>
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

YCbCrEllipseHandThresholder::YCbCrEllipseHandThresholder()
{
    skinCrCbHist = Mat::zeros(Size(256, 256), CV_8UC1);
    ellipse(skinCrCbHist, Point(113, 155.6), Size(23.4, 15.2), 43.0, 0.0, 360.0, Scalar(255, 255, 255), -1);
}

cv::Mat YCbCrEllipseHandThresholder::thresholdHand ( cv::Mat input )
{
    Mat bw = thresholdYCbCr ( input );
    Mat bw2;
    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    //morphologyEx( bw, bw2, MORPH_CLOSE, element );
    //bw = bw2;

    if ( isDebug())
        imshow ( "YCbCrEllipseHandThresholder", bw );

    return bw;
}

cv::Mat YCbCrEllipseHandThresholder::thresholdYCbCr ( cv::Mat input )
{
    Mat ycbcr;
    Mat output = Mat::zeros(input.size(), CV_8UC1);

    cvtColor(input, ycbcr, CV_BGR2YCrCb, 0);

    for(int row = 0; row < input.rows; ++row)
        for(int col = 0; col < input.cols; ++col)
        {
             Vec3b pt = ycbcr.at<Vec3b>(row,col);
             if(pt[0] > 128)
             if ((skinCrCbHist.at<uchar>(pt[1], pt[2]) > 0))
                  output.at<uchar>(row,col) = 255;
        }

    return output;
}

YCbCrEllipseHandThresholder::~YCbCrEllipseHandThresholder()
{
    //dtor
}
