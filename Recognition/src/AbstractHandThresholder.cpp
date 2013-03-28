#include "AbstractHandThresholder.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

AbstractHandThresholder::AbstractHandThresholder()
{

}

cv::Mat AbstractHandThresholder::thresholdHand ( cv::Mat input )
{
   return cv::Mat ();
}

void AbstractHandThresholder::setDebug(char debug)
{
    this->debug = debug;
}

char AbstractHandThresholder::isDebug()
{
    return this->debug;
}
