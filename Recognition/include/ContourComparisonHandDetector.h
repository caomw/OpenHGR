#ifndef CONTOURCOMPARISONHANDDETECTOR_H
#define CONTOURCOMPARISONHANDDETECTOR_H

#include <AbstractHandDetector.h>


class ContourComparisonHandDetector : public AbstractHandDetector
{
    public:
        ContourComparisonHandDetector();
        ~ContourComparisonHandDetector();
        cv::Rect detectHand ( cv::Mat input );
    protected:
    private:
        void loadTemplates ();
        void loadTemplate ( std::string file );

        std::string templates_path = "../data/Templates";
        std::vector<std::vector<cv::Point> > templates;
};

#endif // CONTOURCOMPARISONHANDDETECTOR_H
