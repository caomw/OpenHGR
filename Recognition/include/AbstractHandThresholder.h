#ifndef ABSTRACTHANDTHRESHOLDER_H
#define ABSTRACTHANDTHRESHOLDER_H
#include <AbstractFilter.h>

#include "opencv2/objdetect/objdetect.hpp"

class AbstractHandThresholder : public AbstractFilter
{
    public:
        virtual cv::Mat thresholdHand ( cv::Mat input ) = 0;
    protected:
    private:
};

#endif // ABSTRACTHANDTHRESHOLDER_H
