#ifndef YCBCRELLIPSEHANDTHRESHOLDER_H
#define YCBCRELLIPSEHANDTHRESHOLDER_H

#include <AbstractHandThresholder.h>

#include "opencv2/imgproc/imgproc.hpp"

class YCbCrEllipseHandThresholder : public AbstractHandThresholder
{
    public:
        YCbCrEllipseHandThresholder();
        virtual ~YCbCrEllipseHandThresholder();
        virtual cv::Mat thresholdHand ( cv::Mat input );
    protected:
    private:
        virtual cv::Mat thresholdYCbCr ( cv::Mat input );

    // Morphology parameters
    int morph_elem = cv::MORPH_ELLIPSE;
    int morph_size = 6;

    cv::Mat skinCrCbHist;
};

#endif // YCBCRELLIPSEHANDTHRESHOLDER_H
