#ifndef HAARYCBCRELLIPSEHANDTHRESHOLDER_H
#define HAARYCBCRELLIPSEHANDTHRESHOLDER_H

#include <AbstractHandThresholder.h>

#include "opencv2/imgproc/imgproc.hpp"

class HaarYCbCrEllipseHandThresholder : public AbstractHandThresholder
{
    public:
        HaarYCbCrEllipseHandThresholder();
        virtual ~HaarYCbCrEllipseHandThresholder();
        virtual cv::Mat thresholdHand ( cv::Mat input );
    protected:
    private:
        virtual cv::Mat substractFace ( cv::Mat input );
        virtual cv::Mat thresholdYCbCr ( cv::Mat input );

    // Face detection
    std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
    cv::CascadeClassifier face_cascade;

    // Morphology parameters
    int morph_elem = cv::MORPH_ELLIPSE;
    int morph_size = 6;

    cv::Mat skinCrCbHist;
};

#endif // HAARYCBCRELLIPSEHANDTHRESHOLDER_H
