#include "ContourComparisonHandDetector.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

ContourComparisonHandDetector::ContourComparisonHandDetector()
{
    //ctor
}

ContourComparisonHandDetector::~ContourComparisonHandDetector()
{
    //dtor
}

cv::Rect ContourComparisonHandDetector::detectHand ( cv::Mat input )
{

    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    Canny( input, canny_output, 50, 50*2, 3 );
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    //dint s = findBiggestContour(contours);

    if ( isDebug())
        imshow("Canny", canny_output);

    Mat drawing = Mat::zeros( input.size(), CV_8UC1 );
    for( int i = 0; i< contours.size(); i++ )
    {
       Scalar color = Scalar( rand()%255, rand()%255, rand()%255 );
       drawContours( input, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }

    return cv::Rect(0,0,1,1);
}
