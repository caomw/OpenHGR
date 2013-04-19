#ifndef ALGORITHMTESTER_H
#define ALGORITHMTESTER_H

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

using namespace std;
using namespace boost::filesystem;
using namespace cv;

enum DescriptorDetectorType { SIFT_Detector, SURF_Detector };
enum DescriptorExtractorType { SIFT_Extractor, SURF_Extractor };
enum DescriptorMatcherType { FlannBased };
enum StatisticalModelType { SVM };

class AlgorithmTester
{
    public:
        AlgorithmTester(DescriptorDetectorType descriptorDetectorType, DescriptorExtractorType descriptorExtractorType, DescriptorMatcherType descriptorMatcherType,
                        StatisticalModelType statisticalModelType, int kValue, vector<string> trainingFolders, string testFolder );

        void run();
        virtual ~AlgorithmTester();

    protected:


    private:

        void extractTrainingVocabulary(const path& basepath, bool training);
        void extractBOWDescriptor(const path& basepath, Mat& descriptors, Mat& labels);

        const static int NBR_GESTURE = 5;

        Ptr<DescriptorMatcher> matcher;
        Ptr<DescriptorExtractor> extractor;
        Ptr<FeatureDetector> detector;

        vector<string> trainingFolders;
        string testFolder;

        TermCriteria tc;
        BOWKMeansTrainer bowTrainer;
        BOWImgDescriptorExtractor* bowDE;

        int dictionarySize;
        int retries = 1;
        int flags = KMEANS_PP_CENTERS;
        long cptrKeypointsTraining = 0;
        long cptrImagesTraining = 0;

        // First index is gestureIndex
        // Second index is : 0 for total for this gesture and rest is total of matching against corresponding gesture
        int gesturesCptr[NBR_GESTURE][NBR_GESTURE+1];
};

#endif // ALGORITHMTESTER_H
