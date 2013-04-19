#include "AlgorithmTester.h"

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

#define DEBUG = 0

AlgorithmTester::AlgorithmTester(DescriptorDetectorType descriptorDetectorType, DescriptorExtractorType descriptorExtractorType, DescriptorMatcherType descriptorMatcherType,
                        StatisticalModelType statisticalModelType, int kValue, vector<string> _trainingFolders, string _testFolder):
    dictionarySize(kValue),
    trainingFolders(_trainingFolders),
    testFolder(_testFolder),
    tc(CV_TERMCRIT_ITER, 10, 0.001),
    bowTrainer(dictionarySize,tc,retries,flags)
{
    switch (descriptorDetectorType)
    {
        case SIFT_Detector:
            detector = new SiftFeatureDetector();
            break;
        case SURF_Detector:
        default:
            detector = new SurfFeatureDetector();
            break;
    }

    switch (descriptorExtractorType)
    {
        case SIFT_Detector:
            extractor = DescriptorExtractor::create("SIFT");
            break;
        case SURF_Detector:
        default:
            extractor = DescriptorExtractor::create("SURF");
            break;
    }

    switch (descriptorMatcherType)
    {
        case FlannBased:
        default:
            matcher = DescriptorMatcher::create("FlannBased");
            break;
    }

    bowDE = new BOWImgDescriptorExtractor(extractor,matcher);

}

AlgorithmTester::~AlgorithmTester()
{
    delete bowDE;
}


void AlgorithmTester::extractTrainingVocabulary(const path& basepath, bool training)
{
    for (directory_iterator iter = directory_iterator(basepath); iter
			!= directory_iterator(); iter++) {
		directory_entry entry = *iter;

		if (is_directory(entry.path())) {

			cout << "Processing directory " << entry.path().string() << endl;
			extractTrainingVocabulary(entry.path(),training);

		} else {

			path entryPath = entry.path();
			if (entryPath.extension() == ".pgm" || entryPath.extension() == ".jpg") {

				cout << "Processing file " << entryPath.string() << endl;

				Mat img = imread(entryPath.string());
				if (!img.empty()) {
					vector<KeyPoint> keypoints;
					detector->detect(img, keypoints);
					if (keypoints.empty()) {
						cerr << "Warning: Could not find key points in image: "
								<< entryPath.string() << endl;
					} else {
						Mat features;						extractor->compute(img, keypoints, features);
						cout << "Nbr de keypoints : " << keypoints.size() << endl;

						if (training)
						{
                            cptrKeypointsTraining += keypoints.size();
                            cptrImagesTraining++;
						}
						bowTrainer.add(features);
					}
				} else {
					cerr << "Warning: Could not read image: "
							<< entryPath.string() << endl;
				}

			}
		}
	}
}

void AlgorithmTester::extractBOWDescriptor(const path& basepath, Mat& descriptors, Mat& labels)
{
    for (directory_iterator iter = directory_iterator(basepath); iter
			!= directory_iterator(); iter++) {
		directory_entry entry = *iter;
		if (is_directory(entry.path())) {
			cout << "Processing directory " << entry.path().string() << endl;
			extractBOWDescriptor(entry.path(), descriptors, labels);
		} else {
			path entryPath = entry.path();
			if (entryPath.extension() == ".pgm") {
				cout << "Processing file " << entryPath.string() << endl;
				Mat img = imread(entryPath.string());
				if (!img.empty()) {
					vector<KeyPoint> keypoints;
					detector->detect(img, keypoints);
					if (keypoints.empty()) {
						cerr << "Warning: Could not find key points in image: "
								<< entryPath.string() << endl;
					} else {
						Mat bowDescriptor;
						bowDE->compute(img, keypoints, bowDescriptor);
						descriptors.push_back(bowDescriptor);
						float label=atof(entryPath.filename().c_str());
                        cout << "Label : " << label << endl;
						labels.push_back(label);

						gesturesCptr[label-1][0]++;


					}
				} else {
					cerr << "Warning: Could not read image: "
							<< entryPath.string() << endl;
				}
			}
		}
	}
}


void AlgorithmTester::run()
{

}
