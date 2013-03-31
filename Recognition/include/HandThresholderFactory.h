#ifndef HANDTHRESHOLDERFACTORY_H
#define HANDTHRESHOLDERFACTORY_H

#include <AbstractHandThresholder.h>

enum HandThresholderType { HAAR_HSV, HAAR_HISTOGRAM_HSV, HAAR_YCBCR, HAAR_YCBCR_ELLIPSE, HAAR_ADAPTIVE };

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
