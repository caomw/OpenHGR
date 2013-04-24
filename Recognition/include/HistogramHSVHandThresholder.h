#ifndef HISTOGRAMHSVTHANDHRESHOLDER_H
#define HISTOGRAMHSVTHANDHRESHOLDER_H

#include <AbstractHandThresholder.h>

#include "opencv2/imgproc/imgproc.hpp"

class HistogramHSVHandThresholder : public AbstractHandThresholder
{
    public:
        HistogramHSVHandThresholder();
        virtual ~HistogramHSVHandThresholder();
        virtual cv::Mat thresholdHand ( cv::Mat input );
    protected:
    private:
        int hue_skin_min_value;
        int sat_skin_min_value;

        // Face detection
        std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
        cv::CascadeClassifier face_cascade;

        // Morphology parameters
        int morph_elem = cv::MORPH_ELLIPSE;
        int morph_size = 6;

        // HSV threshold
        cv::Scalar minHSV = cv::Scalar(0,  50, 0);
        cv::Scalar maxHSV = cv::Scalar(20, 190, 255);
};

#endif // HISTOGRAMHSVTHANDHRESHOLDER_H
