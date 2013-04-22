#include "SIFTGestureRecognizer.h"
#include "AbstractLearningModel.h"

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
#include "SVMLearningModel.h"

using namespace cv;
using namespace std;
using namespace boost::filesystem;

SIFTGestureRecognizer::SIFTGestureRecognizer(DescriptorDetectorType descriptorDetectorType, DescriptorExtractorType descriptorExtractorType, DescriptorMatcherType descriptorMatcherType,
                        StatisticalModelType statisticalModelType, int kValue, vector<string> _trainingFolders):
                        dictionarySize(kValue),
    trainingFolders(_trainingFolders),
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
}


SIFTGestureRecognizer::~SIFTGestureRecognizer()
{
    delete bowDE;
    delete bowTrainer;
    delete learner;
}

float SIFTGestureRecognizer::recognizeGesture ( cv::Mat input, cv::Rect roi )
{
    if ( roi.width > 0 )
    {
        Mat hand  = input(roi);
        Mat handGray;

        /*if ( roi.width >= roi.height )
            resize(hand, tmp, Size(miniatureSize, (double)roi.height/roi.width * miniatureSize));
        else
            resize(hand, tmp, Size((double)roi.width/roi.height * miniatureSize, miniatureSize ));*/

        cvtColor( hand, handGray, CV_BGR2GRAY );

        vector<KeyPoint> keypoints;
        detector->detect(handGray, keypoints);

        Mat bowDescriptor;
        bowDE->compute(handGray, keypoints, bowDescriptor);
        return learner->predict(bowDescriptor);

        if (isDebug())
            imshow("SIFTGestureRecognizer", hand);
    }

    return 0.0;
}

void SIFTGestureRecognizer::train()
{
    for(vector<string>::iterator iter=trainingFolders.begin(); iter != trainingFolders.end(); iter++)
    {
        extractTrainingVocabulary(path(*iter),true);
    }

    Mat dictionary = bowTrainer->cluster();
    bowDE->setVocabulary(dictionary);

	cout<<"Processing training data..."<<endl;
	Mat trainingData(0, dictionarySize, CV_32FC1);
	Mat labels(0, 1, CV_32FC1);

    for(vector<string>::iterator iter=trainingFolders.begin(); iter != trainingFolders.end(); iter++)
    {
        extractBOWDescriptor(path(*iter), true, trainingData, labels);
    }

	learner->train(trainingData,labels);
}

void SIFTGestureRecognizer::extractTrainingVocabulary(const path& basepath, bool training)
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

						char tmp = entryPath.filename().c_str()[0];
						float label= atof(&tmp);

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

void SIFTGestureRecognizer::extractBOWDescriptor(const path& basepath, bool training, Mat& descriptors, Mat& labels)
{
    for (directory_iterator iter = directory_iterator(basepath); iter != directory_iterator(); iter++)
    {
		directory_entry entry = *iter;
		if (is_directory(entry.path()))
		{
			//cout << "Processing directory " << entry.path().string() << endl;
			extractBOWDescriptor(entry.path(), training, descriptors, labels);
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
