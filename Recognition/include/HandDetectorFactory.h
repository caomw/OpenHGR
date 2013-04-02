#ifndef HANDDETECTORFACTORY_H
#define HANDDETECTORFACTORY_H

#include <AbstractHandDetector.h>

enum HandDetectorType { CONTOUR_COMPARISON, CONVEX_HULL };

class HandDetectorFactory
{
    public:
        HandDetectorFactory();
        ~HandDetectorFactory();
        AbstractHandDetector* createInstance ( HandDetectorType type );
    protected:
    private:
};

#endif // HANDDETECTORFACTORY_H
