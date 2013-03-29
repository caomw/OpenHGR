#ifndef HANDTHRESHOLDERFACTORY_H
#define HANDTHRESHOLDERFACTORY_H

#include <AbstractHandThresholder.h>

enum HandThresholderType { HAAR_HSV, HAAR_HISTOGRAM_HSV };

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
