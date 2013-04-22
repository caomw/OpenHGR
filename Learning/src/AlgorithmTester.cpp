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
#include <boost/lexical_cast.hpp>

#include "AbstractLearningModel.h"
#include "SVMLearningModel.h"

using namespace std;
using namespace boost::filesystem;
using namespace cv;

#define DEBUG = 0

AlgorithmTester::AlgorithmTester(DescriptorDetectorType descriptorDetectorType, DescriptorExtractorType descriptorExtractorType, DescriptorMatcherType descriptorMatcherType,
                        StatisticalModelType statisticalModelType, int kValue, vector<string> _trainingFolders, string _testFolder, vector<int> _secondTrainingIndex):
    dictionarySize(kValue),
    trainingFolders(_trainingFolders),
    secondTrainingIndex(_secondTrainingIndex),
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
            learnerSecond = new SVMLearningModel();
    }

    bowDE = new BOWImgDescriptorExtractor(extractor,matcher);
    bowTrainer = new BOWKMeansTrainer(dictionarySize,tc,retries,flags);
    bowDESecond = new BOWImgDescriptorExtractor(extractor,matcher);
    bowTrainerSecond = new BOWKMeansTrainer(dictionarySize,tc,retries,flags);



    memset(gesturesCptr, 0, sizeof(gesturesCptr));

}

AlgorithmTester::~AlgorithmTester()
{
    delete bowDE;
    delete bowTrainer;
    delete learner;
    delete learnerSecond;
    delete bowDESecond;
    delete bowTrainerSecond;
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

						char tmp = entryPath.filename().c_str()[0];
						float label= atof(&tmp);

						bowTrainer->add(features);

						if(std::find(secondTrainingIndex.begin(), secondTrainingIndex.end(), (int)label) != secondTrainingIndex.end())
						{
						    bowTrainerSecond->add(features);
						}
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

void AlgorithmTester::extractBOWDescriptor(const path& basepath, bool training, Mat& descriptors, Mat& labels, Mat& descriptors2, Mat& labels2)
{
    for (directory_iterator iter = directory_iterator(basepath); iter != directory_iterator(); iter++)
    {
		directory_entry entry = *iter;
		if (is_directory(entry.path()))
		{
			//cout << "Processing directory " << entry.path().string() << endl;
			extractBOWDescriptor(entry.path(), training, descriptors, labels,descriptors2, labels2);
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

						bowDE->compute(img, keypoints, bowDescriptor);
						descriptors.push_back(bowDescriptor);
						labels.push_back(label);

						if(training && std::find(secondTrainingIndex.begin(), secondTrainingIndex.end(), (int)label) != secondTrainingIndex.end())
						{
						    cout << "Training Second SVM" << endl;
						    Mat bowDescriptor2;
						    bowDESecond->compute(img,keypoints,bowDescriptor2);
						    descriptors2.push_back(bowDescriptor2);
						    labels2.push_back(label);
						}
						else if (!training)
						{
                            filenames.push_back(entryPath.filename().c_str());
						}

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

	cout << "Clustering "<< cptrKeypointsTraining <<" features"<<endl;
	cout << "NBR de features moyen : " << cptrKeypointsTraining/cptrImagesTraining << endl;


    Mat dictionary = bowTrainer->cluster();
    Mat dictionary2;
    cout << "Done clustering" << endl;
    if (secondTrainingIndex.size() > 0)
    {
        dictionary2 = bowTrainerSecond->cluster();
        bowDESecond->setVocabulary(dictionary2);
    }




    bowDE->setVocabulary(dictionary);

	cout<<"Processing training data..."<<endl;
	Mat trainingData(0, dictionarySize, CV_32FC1);
	Mat labels(0, 1, CV_32FC1);
	Mat trainingData2(0, dictionarySize, CV_32FC1);
	Mat labels2(0, 1, CV_32FC1);

    for(vector<string>::iterator iter=trainingFolders.begin(); iter != trainingFolders.end(); iter++)
    {
        extractBOWDescriptor(path(*iter), true, trainingData, labels,trainingData2,labels2);
    }


	cout << "Training classifier.... using SVM" << endl;
	learner->train(trainingData,labels);

	if (secondTrainingIndex.size() > 0)
        learnerSecond->train(trainingData2,labels2);

	timeElapsedLearning = timerLearning.elapsed();

	boost::timer timerTesting;

    cout<<"Processing evaluation data..."<<endl;
	Mat evalData(0, dictionarySize, CV_32FC1);
	Mat groundTruth(0, 1, CV_32FC1);
	extractBOWDescriptor(path(testFolder), false, evalData, groundTruth, trainingData2, labels2);

	Mat results;

	for (int i=0; i < evalData.rows; i++)
	{
	    float result = learner->predict(evalData.row(i));

	    if(std::find(secondTrainingIndex.begin(), secondTrainingIndex.end(), (int)result) != secondTrainingIndex.end())
	    {
	        float resultSecond = learnerSecond->predict(evalData.row(i));
	        if (result != resultSecond)
	        {
	            result = resultSecond;
                gesturesChangedcptr++;
	        }

	    }

        results.push_back(result);


	}

	timeElapsedTesting = timerTesting.elapsed();

    //Evaluation stats
    errorRate = (double) countNonZero(groundTruth - results) / evalData.rows;

    for(int i=0; i < groundTruth.rows; i++)
    {
        gesturesCptr[(int)groundTruth.at<float>(i,0)-1][(int)(results.at<float>(i,0))]++;
        gesturesCptr[(int)groundTruth.at<float>(i,0)-1][0]++;


        if ((int)groundTruth.at<float>(i,0) != (int)results.at<float>(i,0) && ((int)results.at<float>(i,0) == 1 || (int)results.at<float>(i,0) == 5))
        {
            string tmp = "" + boost::lexical_cast<std::string>((int)groundTruth.at<float>(i,0)) + "-" + boost::lexical_cast<std::string>((int)results.at<float>(i,0)) + "-" + boost::lexical_cast<std::string>(filenames.at(i));
            wrongGestures.push_back(tmp);
        }

    }



	cout << "Error rate: " << errorRate << endl;

	cout << "Time Elapsed : " << timerLearning.elapsed() << endl;


}
