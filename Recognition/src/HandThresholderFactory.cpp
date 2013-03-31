#include <HandThresholderFactory.h>
#include <HaarHSVHandThresholder.h>
#include <HaarHistogramHSVHandThresholder.h>
#include <HaarYCbCrHandThresholder.h>
#include <HaarYCbCrEllipseHandThresholder.h>
#include <HaarAdaptiveHandThresholder.h>
#include <HaarLumaHandThresholder.h>

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
        case HAAR_YCBCR:
            return new HaarYCbCrHandThresholder();
        case HAAR_YCBCR_ELLIPSE:
            return new HaarYCbCrEllipseHandThresholder();
        case HAAR_ADAPTIVE:
            return new HaarAdaptiveHandThresholder();
        case HAAR_LUMA:
            return new HaarLumaHandThresholder();
        case HAAR_HSV:
        default: return new HaarHSVHandThresholder ();
    }
}


