#include "SIFTGestureRecognizer.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

SIFTGestureRecognizer::SIFTGestureRecognizer()
{
    //ctor
}

std::string SIFTGestureRecognizer::recognizeGesture ( cv::Mat input, cv::Rect roi )
{
    if ( roi.width > 0 )
    {
        Mat hand  = input(roi);
        Mat tmp;

        if ( roi.width >= roi.height )
            resize(hand, tmp, Size(miniatureSize, (double)roi.height/roi.width * miniatureSize));
        else
            resize(hand, tmp, Size((double)roi.width/roi.height * miniatureSize, miniatureSize ));

        cvtColor( tmp, hand, CV_BGR2GRAY );

        if (isDebug())
            imshow("SIFTGestureRecognizer", hand);
    }

    return "";
}

SIFTGestureRecognizer::~SIFTGestureRecognizer()
{
    //dtor
}
