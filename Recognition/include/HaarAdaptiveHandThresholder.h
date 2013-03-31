#ifndef HAARADAPTIVEHANDTHRESHOLDER_H
#define HAARADAPTIVEHANDTHRESHOLDER_H

#include <AbstractHandThresholder.h>

#include "opencv2/imgproc/imgproc.hpp"

class HaarAdaptiveHandThresholder : public AbstractHandThresholder
{
    public:
        HaarAdaptiveHandThresholder();
        virtual ~HaarAdaptiveHandThresholder();
        virtual cv::Mat thresholdHand ( cv::Mat input );
    protected:
    private:
        void displayBuffer(IplImage *rgbDestImage, IplImage *buffer, int rValue, int gValue, int bValue);

        IplImage *filterMask = NULL;
};

#endif // HAARADAPTIVEHANDTHRESHOLDER_H
