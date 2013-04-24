#ifndef HSVHANDTHRESHOLDER_H
#define HSVHANDTHRESHOLDER_H

#include <AbstractHandThresholder.h>
#include <iostream>
#include <stdio.h>

#include "opencv2/imgproc/imgproc.hpp"

class HSVHandThresholder : public AbstractHandThresholder
{
    public:
        HSVHandThresholder();
        virtual ~HSVHandThresholder();
        virtual cv::Mat thresholdHand ( cv::Mat input );
    protected:
    private:
        void fillHoles ( cv::Mat input );

        // Morphology parameters
        int morph_elem = cv::MORPH_ELLIPSE;
        int morph_size = 6;

        // HSV threshold
        cv::Scalar minHSV1 = cv::Scalar(0,50,0);
        cv::Scalar maxHSV1 = cv::Scalar(19,200,255);
        cv::Scalar minHSV2 = cv::Scalar(150,50,0);
        cv::Scalar maxHSV2 = cv::Scalar(180,200,255);
};

#endif // HSVHANDTHRESHOLDER_H
