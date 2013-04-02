#ifndef HAARHSVHANDTHRESHOLDER_H
#define HAARHSVHANDTHRESHOLDER_H

#include <AbstractHandThresholder.h>
#include <iostream>
#include <stdio.h>

#include "opencv2/imgproc/imgproc.hpp"

class HaarHSVHandThresholder : public AbstractHandThresholder
{
    public:
        HaarHSVHandThresholder();
        virtual ~HaarHSVHandThresholder();
        virtual cv::Mat thresholdHand ( cv::Mat input );
    protected:
        cv::Mat substractFace ( cv::Mat input );
    private:
        void fillHoles ( cv::Mat input );

        // Face detection
        std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
        cv::CascadeClassifier face_cascade;

        // Morphology parameters
        int morph_elem = cv::MORPH_ELLIPSE;
        int morph_size = 6;

        // HSV threshold
        cv::Scalar minHSV1 = cv::Scalar(0,50,0);
        cv::Scalar maxHSV1 = cv::Scalar(19,200,255);
        cv::Scalar minHSV2 = cv::Scalar(150,50,0);
        cv::Scalar maxHSV2 = cv::Scalar(180,200,255);
};

#endif // HAARHSVHANDTHRESHOLDER_H
