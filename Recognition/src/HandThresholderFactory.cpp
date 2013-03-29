#include <HandThresholderFactory.h>
#include <HaarHSVHandThresholder.h>
#include <HaarHistogramHSVHandThresholder.h>

HandThresholderFactory::HandThresholderFactory()
{
    //ctor
}

HandThresholderFactory::~HandThresholderFactory()
{
    //ctor
}

AbstractHandThresholder* HandThresholderFactory::createInstance ( HandThresholderType type )
{
    switch ( type )
    {
        case HAAR_HISTOGRAM_HSV:
            return new HaarHistogramHSVHandThresholder();
        case HAAR_HSV:
        default: return new HaarHSVHandThresholder ();
    }
}


