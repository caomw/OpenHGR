#ifndef HAARLUMAHANDTHRESHOLDER_H
#define HAARLUMAHANDTHRESHOLDER_H

#include <AbstractHandThresholder.h>

#include "opencv2/imgproc/imgproc.hpp"

class LumaHandThresholder : public AbstractHandThresholder
{
    public:
        LumaHandThresholder();
        virtual ~LumaHandThresholder();
        virtual cv::Mat thresholdHand ( cv::Mat input );
    protected:
    private:

    // Morphology parameters
    int morph_elem = cv::MORPH_ELLIPSE;
    int morph_size = 6;
};

#endif // LUMAHANDTHRESHOLDER_H
