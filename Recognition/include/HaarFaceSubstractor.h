#ifndef HAARFACESUBSTRACTOR_H
#define HAARFACESUBSTRACTOR_H

#include <AbstractFaceSubstractor.h>

#include "opencv2/imgproc/imgproc.hpp"

class HaarFaceSubstractor : public AbstractFaceSubstractor
{
    public:
        HaarFaceSubstractor();
        virtual ~HaarFaceSubstractor();
        virtual cv::Mat substractFace ( cv::Mat input, cv::Mat thresholdFrame );
    protected:
    private:
        std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
        cv::CascadeClassifier face_cascade;
};

#endif // HAARFACESUBSTRACTOR_H
