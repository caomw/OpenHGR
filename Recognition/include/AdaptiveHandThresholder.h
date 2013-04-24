#ifndef ADAPTIVEHANDTHRESHOLDER_H
#define ADAPTIVEHANDTHRESHOLDER_H

#include <AbstractHandThresholder.h>

#include "opencv2/imgproc/imgproc.hpp"

class AdaptiveHandThresholder : public AbstractHandThresholder
{
    public:
        AdaptiveHandThresholder();
        virtual ~AdaptiveHandThresholder();
        virtual cv::Mat thresholdHand ( cv::Mat input );
    protected:
    private:
        IplImage *filterMask = NULL;

    // Morphology parameters
    int morph_elem = cv::MORPH_ELLIPSE;
    int morph_size = 6;
};

#endif // ADAPTIVEHANDTHRESHOLDER_H
