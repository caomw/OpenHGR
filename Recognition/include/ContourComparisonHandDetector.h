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
        std::vector<cv::Point> cropContour ( std::vector<cv::Point> contour, cv::Rect rect );

        std::string templates_path = "../data/Templates";
        std::vector<std::vector<cv::Point> > templates;

        double shapeMatchThreshold = 0.20;
        double widthHeightRadio = 1.2;
};

#endif // CONTOURCOMPARISONHANDDETECTOR_H
