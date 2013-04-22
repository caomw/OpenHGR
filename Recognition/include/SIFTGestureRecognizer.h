#ifndef SIFTGESTURERECOGNIZER_H
#define SIFTGESTURERECOGNIZER_H

#include <vector>
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ml/ml.hpp"

#include <AbstractGestureRecognizer.h>
#include "AbstractLearningModel.h"



using namespace std;
using namespace cv;
using namespace boost::filesystem;


enum DescriptorDetectorType { SIFT_Detector, SURF_Detector };
enum DescriptorExtractorType { SIFT_Extractor, SURF_Extractor };
enum DescriptorMatcherType { FlannBased };
enum StatisticalModelType { SVM_Model };

class SIFTGestureRecognizer : public AbstractGestureRecognizer
{
    public:
        SIFTGestureRecognizer(DescriptorDetectorType descriptorDetectorType, DescriptorExtractorType descriptorExtractorType, DescriptorMatcherType descriptorMatcherType,
                        StatisticalModelType statisticalModelType, int kValue, vector<string> trainingFolders);
        virtual ~SIFTGestureRecognizer();
        float recognizeGesture ( cv::Mat input, cv::Rect roi );
        void train();
    protected:
    private:

        void extractTrainingVocabulary(const path& basepath, bool training);
        void extractBOWDescriptor(const path& basepath, bool training, Mat& descriptors, Mat& labels);



        Ptr<DescriptorMatcher> matcher;
        Ptr<DescriptorExtractor> extractor;
        Ptr<FeatureDetector> detector;

        vector<string> trainingFolders;
        vector<int> secondTrainingIndex;
        string testFolder;

        TermCriteria tc;
        BOWKMeansTrainer* bowTrainer;
        BOWImgDescriptorExtractor* bowDE;

        BOWKMeansTrainer* bowTrainerSecond;
        BOWImgDescriptorExtractor* bowDESecond;

        int dictionarySize;
        int retries = 1;
        int flags = KMEANS_PP_CENTERS;


        AbstractLearningModel* learner;
        int miniatureSize = 100;
        int kValues;
        vector<string> trainingDirectories;

};

#endif // SIFTGESTURERECOGNIZER_H
