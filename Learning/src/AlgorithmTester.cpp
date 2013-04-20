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
#include <boost/timer.hpp>

#include "AbstractLearningModel.h"
#include "SVMLearningModel.h"

using namespace std;
using namespace boost::filesystem;
using namespace cv;

#define DEBUG = 0

AlgorithmTester::AlgorithmTester(DescriptorDetectorType descriptorDetectorType, DescriptorExtractorType descriptorExtractorType, DescriptorMatcherType descriptorMatcherType,
                        StatisticalModelType statisticalModelType, int kValue, vector<string> _trainingFolders, string _testFolder):
    dictionarySize(kValue),
    trainingFolders(_trainingFolders),
    testFolder(_testFolder),
    tc(CV_TERMCRIT_ITER, 10, 0.001)
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

    switch(statisticalModelType)
    {
        case SVM_Model:
        default:
            learner = new SVMLearningModel();
    }

    bowDE = new BOWImgDescriptorExtractor(extractor,matcher);
    bowTrainer = new BOWKMeansTrainer(dictionarySize,tc,retries,flags);

    memset(gesturesCptr, 0, sizeof(gesturesCptr));

}

AlgorithmTester::~AlgorithmTester()
{
    delete bowDE;
    delete bowTrainer;
    delete learner;
}


void AlgorithmTester::extractTrainingVocabulary(const path& basepath, bool training)
{
    for (directory_iterator iter = directory_iterator(basepath); iter != directory_iterator(); iter++)
    {
		directory_entry entry = *iter;

		if (is_directory(entry.path()))
		{

			//cout << "Processing directory " << entry.path().string() << endl;
			extractTrainingVocabulary(entry.path(),training);

		}
        else
        {

			path entryPath = entry.path();
			if (entryPath.extension() == ".pgm" || entryPath.extension() == ".jpg")
			{

				//cout << "Processing file " << entryPath.string() << endl;

				Mat img = imread(entryPath.string());

				if (!img.empty())
				{
					vector<KeyPoint> keypoints;
					detector->detect(img, keypoints);

					if (keypoints.empty())
					{
						cerr << "Warning: Could not find key points in image: " << entryPath.string() << endl;
					}
					else
					{
						Mat features;
						extractor->compute(img, keypoints, features);
						//cout << "Nbr de keypoints : " << keypoints.size() << endl;

						if (training)
						{
                            cptrKeypointsTraining += keypoints.size();
                            cptrImagesTraining++;
						}
						bowTrainer->add(features);
					}
				}
				else
				{
					cerr << "Warning: Could not read image: " << entryPath.string() << endl;
				}

			}
		}
	}
}

void AlgorithmTester::extractBOWDescriptor(const path& basepath, Mat& descriptors, Mat& labels)
{
    for (directory_iterator iter = directory_iterator(basepath); iter != directory_iterator(); iter++)
    {
		directory_entry entry = *iter;
		if (is_directory(entry.path()))
		{
			//cout << "Processing directory " << entry.path().string() << endl;
			extractBOWDescriptor(entry.path(), descriptors, labels);
		}
		else
		{
			path entryPath = entry.path();

			if (entryPath.extension() == ".pgm" || entryPath.extension() == ".jpg")
			{
				//cout << "Processing file " << entryPath.string() << endl;
				Mat img = imread(entryPath.string());

				if (!img.empty())
				{

					vector<KeyPoint> keypoints;
					detector->detect(img, keypoints);

					if (keypoints.empty())
					{
						cerr << "Warning: Could not find key points in image: " << entryPath.string() << endl;
					}
					else
					{
						Mat bowDescriptor;
						char tmp = entryPath.filename().c_str()[0];
						float label= atof(&tmp);
						cout << "Label : " << label << endl;

						bowDE->compute(img, keypoints, bowDescriptor);
						descriptors.push_back(bowDescriptor);
						labels.push_back(label);

					}
				}
				else
				{
					cerr << "Warning: Could not read image: " << entryPath.string() << endl;
				}
			}
		}
	}
}


void AlgorithmTester::run()
{
    boost::timer timerLearning;

    cout << "Creating dictionary..." << endl;

    for(vector<string>::iterator iter=trainingFolders.begin(); iter != trainingFolders.end(); iter++)
    {
        extractTrainingVocabulary(path(*iter),true);
    }

	vector<Mat> descriptors = bowTrainer->getDescriptors();

	cout << "Clustering "<< cptrKeypointsTraining <<" features"<<endl;
	cout << "NBR de features moyen : " << cptrKeypointsTraining/cptrImagesTraining << endl;


    Mat dictionary = bowTrainer->cluster();



    cout << "Done clustering" << endl;
    bowDE->setVocabulary(dictionary);

	cout<<"Processing training data..."<<endl;
	Mat trainingData(0, dictionarySize, CV_32FC1);

	Mat labels(0, 1, CV_32FC1);

    for(vector<string>::iterator iter=trainingFolders.begin(); iter != trainingFolders.end(); iter++)
    {
        extractBOWDescriptor(path(*iter), trainingData, labels);
    }


	cout << "Training classifier.... using SVM" << endl;
	learner->train(trainingData,labels);

	timeElapsedLearning = timerLearning.elapsed();

	boost::timer timerTesting;

    cout<<"Processing evaluation data..."<<endl;
	Mat evalData(0, dictionarySize, CV_32FC1);
	Mat groundTruth(0, 1, CV_32FC1);
	extractBOWDescriptor(path(testFolder), evalData, groundTruth);

	Mat results;

	for (int i=0; i < evalData.rows; i++)
	{
        results.push_back(learner->predict(evalData.row(i)));
	}

	timeElapsedTesting = timerTesting.elapsed();

    //Evaluation stats
    errorRate = (double) countNonZero(groundTruth - results) / evalData.rows;

    for(int i=0; i < groundTruth.rows; i++)
    {
        gesturesCptr[(int)groundTruth.at<float>(i,0)-1][(int)(results.at<float>(i,0))]++;
        gesturesCptr[(int)groundTruth.at<float>(i,0)-1][0]++;

    }



	cout << "Error rate: " << errorRate << endl;

	cout << "Time Elapsed : " << timerLearning.elapsed() << endl;


}
