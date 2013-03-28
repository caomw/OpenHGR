#ifndef HANDTHRESHOLDERFACTORY_H
#define HANDTHRESHOLDERFACTORY_H

#include <AbstractHandThresholder.h>

class HandThresholderFactory
{
    public:
        HandThresholderFactory();
        AbstractHandThresholder* getHandThresholder ( int type );
    protected:
    private:
};

 enum { HAAR_HSV, HAAR_HISTOGRAM_HSV } HandThresholderType;

#endif // HANDTHRESHOLDERFACTORY_H
