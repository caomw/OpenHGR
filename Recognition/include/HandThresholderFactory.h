#ifndef HANDTHRESHOLDERFACTORY_H
#define HANDTHRESHOLDERFACTORY_H

#include <AbstractHandThresholder.h>

enum HandThresholderType { HSV_THRESHOLDER, HISTOGRAM_HSV_THRESHOLDER, YCBCR_THRESHOLDER, YCBCR_ELLIPSE_THRESHOLDER, ADAPTIVE_THRESHOLDER, LUMA_THRESHOLDER };

class HandThresholderFactory
{
    public:
        HandThresholderFactory();
        ~HandThresholderFactory();
        AbstractHandThresholder* createInstance ( HandThresholderType type );
    protected:
    private:
};

#endif // HANDTHRESHOLDERFACTORY_H
