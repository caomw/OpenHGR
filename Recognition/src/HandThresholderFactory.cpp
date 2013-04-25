#include <HandThresholderFactory.h>
#include <HSVHandThresholder.h>
#include <HistogramHSVHandThresholder.h>
#include <YCbCrHandThresholder.h>
#include <YCbCrEllipseHandThresholder.h>
#include <AdaptiveHandThresholder.h>
#include <LumaHandThresholder.h>
#include <LumaMeanHandThresholder.h>

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
        case HISTOGRAM_HSV_THRESHOLDER:
            return new HistogramHSVHandThresholder();
        case YCBCR_THRESHOLDER:
            return new YCbCrHandThresholder();
        case YCBCR_ELLIPSE_THRESHOLDER:
            return new YCbCrEllipseHandThresholder();
        case ADAPTIVE_THRESHOLDER:
            return new AdaptiveHandThresholder();
        case LUMA_THRESHOLDER:
            return new LumaHandThresholder();
        case LUMA_MEAN_THRESHOLDER:
            return new LumaMeanHandThresholder();
        case HSV_THRESHOLDER:
        default: return new HSVHandThresholder ();
    }
}


