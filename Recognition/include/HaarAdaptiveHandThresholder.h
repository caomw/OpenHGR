#ifndef HAARADAPTIVEHANDTHRESHOLDER_H
#define HAARADAPTIVEHANDTHRESHOLDER_H

#include <AbstractHandThresholder.h>

#include "opencv2/imgproc/imgproc.hpp"

class HaarAdaptiveHandThresholder : public AbstractHandThresholder
{
    public:
        HaarAdaptiveHandThresholder();
        virtual ~HaarAdaptiveHandThresholder();
        virtual cv::Mat thresholdHand ( cv::Mat input );
    protected:
    private:
        cv::Mat substractFace ( cv::Mat input );

        IplImage *filterMask = NULL;

        // Face detection
        std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
        cv::CascadeClassifier face_cascade;


    // Morphology parameters
    int morph_elem = cv::MORPH_ELLIPSE;
    int morph_size = 6;
};

#endif // HAARADAPTIVEHANDTHRESHOLDER_H
