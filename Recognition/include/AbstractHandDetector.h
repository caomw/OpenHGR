#ifndef ABSTRACTHANDDETECTOR_H
#define ABSTRACTHANDDETECTOR_H
#include <AbstractFilter.h>

#include "opencv2/objdetect/objdetect.hpp"

class AbstractHandDetector : public AbstractFilter
{
    public:
        virtual cv::Rect detectHand ( cv::Mat input ) = 0;
    protected:
    private:
};

#endif // ABSTRACTHANDDETECTOR_H
