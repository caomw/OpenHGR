#ifndef SIFTGESTURERECOGNIZER_H
#define SIFTGESTURERECOGNIZER_H

#include <AbstractGestureRecognizer.h>


class SIFTGestureRecognizer : public AbstractGestureRecognizer
{
    public:
        SIFTGestureRecognizer();
        virtual ~SIFTGestureRecognizer();
        virtual std::string recognizeGesture ( cv::Mat input, cv::Rect roi );
    protected:
    private:
        int miniatureSize = 100;
};

#endif // SIFTGESTURERECOGNIZER_H
