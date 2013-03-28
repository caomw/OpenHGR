#ifndef ABSTRACTHANDTHRESHOLDER_H
#define ABSTRACTHANDTHRESHOLDER_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class AbstractHandThresholder
{
    public:
        AbstractHandThresholder();
        virtual cv::Mat thresholdHand ( cv::Mat input );
        void setDebug (char debug);
    protected:
        char isDebug ();
    private:
        char debug = 0;
};

#endif // ABSTRACTHANDTHRESHOLDER_H
