#include "SVMLearningModel.h"

#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"

SVMLearningModel::SVMLearningModel(){}

float SVMLearningModel::predict(const cv::Mat& sample)
{
    return svm.predict(sample);
}

void SVMLearningModel::train(const cv::Mat& trainData, const cv::Mat& responses)
{
    svm.train(trainData,responses);
}
