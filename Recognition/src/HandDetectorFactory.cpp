#include <HandDetectorFactory.h>

#include <AbstractHandDetector.h>
#include <ContourComparisonHandDetector.h>

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
        case CONTOUR_COMPARISON:
        default: return new ContourComparisonHandDetector ();
    }
}
