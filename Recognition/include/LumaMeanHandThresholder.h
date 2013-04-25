#ifndef LUMAMEANHANDTHRESHOLDER_H
#define LUMAMEANHANDTHRESHOLDER_H

#include <AbstractHandThresholder.h>

#include "opencv2/imgproc/imgproc.hpp"

class LumaMeanHandThresholder : public AbstractHandThresholder
{
    public:
        LumaMeanHandThresholder();
        virtual ~LumaMeanHandThresholder();
        virtual cv::Mat thresholdHand ( cv::Mat input );
    protected:
    private:

    // Morphology parameters
    int morph_elem = cv::MORPH_ELLIPSE;
    int morph_size = 6;
};

#endif // LUMAMEANHANDTHRESHOLDER_H
