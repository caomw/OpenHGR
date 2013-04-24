#ifndef YCBCRHANDTHRESHOLDER_H
#define YCBCRHANDTHRESHOLDER_H

#include <AbstractHandThresholder.h>

#include "opencv2/imgproc/imgproc.hpp"

class YCbCrHandThresholder : public AbstractHandThresholder
{
    public:
        YCbCrHandThresholder();
        virtual ~YCbCrHandThresholder();
        virtual cv::Mat thresholdHand ( cv::Mat input );
    protected:
    private:

    // Morphology parameters
    int morph_elem = cv::MORPH_ELLIPSE;
    int morph_size = 6;
};

#endif // YCBCRHANDTHRESHOLDER_H
