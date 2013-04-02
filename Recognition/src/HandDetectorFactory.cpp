#include <HandDetectorFactory.h>

#include <AbstractHandDetector.h>
#include <ContourComparisonHandDetector.h>
#include <ConvexHullComparisonHandDetector.h>

HandDetectorFactory::HandDetectorFactory()
{
    //ctor
}

HandDetectorFactory::~HandDetectorFactory()
{
    //dtor
}

AbstractHandDetector* HandDetectorFactory::createInstance ( HandDetectorType type )
{
    switch ( type )
    {
        case CONVEX_HULL:
            return new ConvexHullComparisonHandDetector();
        case CONTOUR_COMPARISON:
        default: return new ContourComparisonHandDetector ();
    }
}
