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
        std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
        cv::CascadeClassifier face_cascade;
};

#endif // HAARHSVHANDTHRESHOLDER_H
