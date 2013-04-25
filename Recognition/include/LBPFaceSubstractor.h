#ifndef LBPFACESUBSTRACTOR_H
#define LBPFACESUBSTRACTOR_H

#include <AbstractFaceSubstractor.h>

#include "opencv2/imgproc/imgproc.hpp"

class LBPFaceSubstractor : public AbstractFaceSubstractor
{
    public:
        LBPFaceSubstractor();
        virtual ~LBPFaceSubstractor();
        virtual cv::Mat substractFace ( cv::Mat input, cv::Mat thresholdFrame );
    protected:
    private:
        std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
        cv::CascadeClassifier face_cascade;
};

#endif // LBPFACESUBSTRACTOR_H
