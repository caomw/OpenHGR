#ifndef ABSTRACTFACESUBSTRACTOR_H
#define ABSTRACTFACESUBSTRACTOR_H

#include <AbstractFilter.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"

class AbstractFaceSubstractor : public AbstractFilter
{
    public:
        virtual cv::Mat substractFace ( cv::Mat input, cv::Mat thresholdFrame ) = 0;
    protected:
    private:
};

#endif // ABSTRACTFACESUBSTRACTOR_H
