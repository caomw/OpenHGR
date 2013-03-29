#ifndef CONTOURCOMPARISONHANDDETECTOR_H
#define CONTOURCOMPARISONHANDDETECTOR_H

#include <AbstractHandDetector.h>


class ContourComparisonHandDetector : public AbstractHandDetector
{
    public:
        ContourComparisonHandDetector();
        ~ContourComparisonHandDetector();
        cv::Rect detectHand ( cv::Mat input );
    protected:
    private:
};

#endif // CONTOURCOMPARISONHANDDETECTOR_H
