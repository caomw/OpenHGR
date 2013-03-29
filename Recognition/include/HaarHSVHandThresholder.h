#ifndef HAARHSVHANDTHRESHOLDER_H
#define HAARHSVHANDTHRESHOLDER_H

#include <AbstractHandThresholder.h>
#include <iostream>
#include <stdio.h>

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


        std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
        int erosionSize = 1;
        int cannyThreshold = 50;
        int holeFillingIterations = 3;

        cv::Scalar minHSV = cv::Scalar(0,  50, 0);
        cv::Scalar maxHSV = cv::Scalar(20, 190, 255);

        cv::CascadeClassifier face_cascade;

};

#endif // HAARHSVHANDTHRESHOLDER_H
