#ifndef CONVEXHULLCOMPARISONHANDDETECTOR_H
#define CONVEXHULLCOMPARISONHANDDETECTOR_H

#include <AbstractHandDetector.h>

class ConvexHullComparisonHandDetector: public AbstractHandDetector
{
    public:
        ConvexHullComparisonHandDetector();
        virtual ~ConvexHullComparisonHandDetector();
         cv::Rect detectHand ( cv::Mat input );
    protected:
    private:
};

#endif // CONVEXHULLCOMPARISONHANDDETECTOR_H
