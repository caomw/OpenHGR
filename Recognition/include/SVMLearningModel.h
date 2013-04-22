#ifndef SVMLEARNINGMODEL_H
#define SVMLEARNINGMODEL_H

#include "AbstractLearningModel.h"
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"


class SVMLearningModel : public AbstractLearningModel
{
    public:
        SVMLearningModel();
        float predict(const cv::Mat& sample);
        void train(const cv::Mat& trainData, const cv::Mat& responses);
    protected:
    private:
        CvSVM svm;

};

#endif // SVMLEARNINGMODEL_H
