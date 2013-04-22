#ifndef ABSTRACTLEARNINGMODEL_H
#define ABSTRACTLEARNINGMODEL_H

#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"

class AbstractLearningModel
{
    public:
        virtual float predict(const cv::Mat& sample) = 0;
        virtual void train(const cv::Mat& trainData, const cv::Mat& responses) =0;
    protected:
    private:
};

#endif // ABSTRACTLEARNINGMODEL_H
