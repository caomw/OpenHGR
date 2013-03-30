#ifndef ABSTRACTGESTURERECOGNIZER_H
#define ABSTRACTGESTURERECOGNIZER_H

#include <AbstractFilter.h>

#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>

class AbstractGestureRecognizer : public AbstractFilter
{
    public:
        virtual std::string recognizeGesture ( cv::Mat input, cv::Rect roi ) = 0;
    protected:
    private:
};

#endif // ABSTRACTGESTURERECOGNIZER_H
