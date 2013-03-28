#ifndef HAARHISTOGRAMHSVTHANDHRESHOLDER_H
#define HAARHISTOGRAMHSVTHANDHRESHOLDER_H

#include <AbstractHandThresholder.h>


class HaarHistogramHSVHandThresholder : public AbstractHandThresholder
{
    public:
        HaarHistogramHSVHandThresholder();
        virtual ~HaarHistogramHSVHandThresholder();
        virtual cv::Mat thresholdHand ( cv::Mat input );
    protected:
    private:
        std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
        cv::CascadeClassifier face_cascade;
        int hue_skin_min_value;
        int sat_skin_min_value;
};

#endif // HAARHISTOGRAMHSVTHANDHRESHOLDER_H
