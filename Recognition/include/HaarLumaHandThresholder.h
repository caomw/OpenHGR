#ifndef HAARLUMAHANDTHRESHOLDER_H
#define HAARLUMAHANDTHRESHOLDER_H

#include <AbstractHandThresholder.h>

#include "opencv2/imgproc/imgproc.hpp"

class HaarLumaHandThresholder : public AbstractHandThresholder
{
    public:
        HaarLumaHandThresholder();
        virtual ~HaarLumaHandThresholder();
        virtual cv::Mat thresholdHand ( cv::Mat input );
    protected:
    private:
        cv::Mat substractFace ( cv::Mat input );

    // Face detection
    std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
    cv::CascadeClassifier face_cascade;

    // Morphology parameters
    int morph_elem = cv::MORPH_ELLIPSE;
    int morph_size = 6;
};

#endif // HAARLUMAHANDTHRESHOLDER_H