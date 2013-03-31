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

//location of the training data
#define TRAINING_DATA_DIR "/home/frederick/Code/OpenHGR/data/train/Cropped/"
//location of the evaluation data
#define EVAL_DATA_DIR "/home/frederick/Code/OpenHGR/data/eval/Cropped"

//See article on BoW model for details
Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");
Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create("SURF");
//Ptr<FeatureDetector> detector = FeatureDetector::create("SURF");

//See article on BoW model for details
int dictionarySize = 1000;
TermCriteria tc(CV_TERMCRIT_ITER, 10, 0.001);
int retries = 1;
int flags = KMEANS_PP_CENTERS;

//See article on BoW model for details
BOWKMeansTrainer bowTrainer(dictionarySize, tc, retries, flags);
//See article on BoW model for details
BOWImgDescriptorExtractor bowDE(extractor, matcher);

/**
 * \brief Recursively traverses a folder hierarchy. Extracts features from the training images and adds them to the bowTrainer.
 */
void extractTrainingVocabulary(const path& basepath) {
    SurfFeatureDetector detector;
	for (directory_iterator iter = directory_iterator(basepath); iter
			!= directory_iterator(); iter++) {
		directory_entry entry = *iter;

		if (is_directory(entry.path())) {

			cout << "Processing directory " << entry.path().string() << endl;
			extractTrainingVocabulary(entry.path());

		} else {

			path entryPath = entry.path();
			if (entryPath.extension() == ".pgm") {

				cout << "Processing file " << entryPath.string() << endl;

				Mat img = imread(entryPath.string());
				if (!img.empty()) {
					vector<KeyPoint> keypoints;
					detector.detect(img, keypoints);
					if (keypoints.empty()) {
						cerr << "Warning: Could not find key points in image: "
								<< entryPath.string() << endl;
					} else {
						Mat features;						extractor->compute(img, keypoints, features);
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

/**
 * \brief Recursively traverses a folder hierarchy. Creates a BoW descriptor for each image encountered.
 */
void extractBOWDescriptor(const path& basepath, Mat& descriptors, Mat& labels) {
    SurfFeatureDetector detector;
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
					detector.detect(img, keypoints);
					if (keypoints.empty()) {
						cerr << "Warning: Could not find key points in image: "
								<< entryPath.string() << endl;
					} else {
						Mat bowDescriptor;
						bowDE.compute(img, keypoints, bowDescriptor);
						descriptors.push_back(bowDescriptor);
						float label=atof(entryPath.filename().c_str());
                        cout << "Label : " << label << endl;
						labels.push_back(label);
					}
				} else {
					cerr << "Warning: Could not read image: "
							<< entryPath.string() << endl;
				}
			}
		}
	}
}

int main(int argc, char ** argv) {

	cout<<"Creating dictionary..."<<endl;
	extractTrainingVocabulary(path(TRAINING_DATA_DIR));
	vector<Mat> descriptors = bowTrainer.getDescriptors();
	int count=0;
	for(vector<Mat>::iterator iter=descriptors.begin();iter!=descriptors.end();iter++)
	{
		count+=iter->rows;
	}
	cout<<"Clustering "<<count<<" features"<<endl;
	Mat dictionary = bowTrainer.cluster();
	bowDE.setVocabulary(dictionary);
	cout<<"Processing training data..."<<endl;
	Mat trainingData(0, dictionarySize, CV_32FC1);
	Mat labels(0, 1, CV_32FC1);
	extractBOWDescriptor(path(TRAINING_DATA_DIR), trainingData, labels);


    cout<<"Processing evaluation data..."<<endl;
	Mat evalData(0, dictionarySize, CV_32FC1);
	Mat groundTruth(0, 1, CV_32FC1);
	extractBOWDescriptor(path(EVAL_DATA_DIR), evalData, groundTruth);

    //Normal Bayes Classifier
	/*NormalBayesClassifier classifier;
	cout<<"Training classifier... using Normal Bayes Classifier"<<endl;

	classifier.train(trainingData, labels);



	cout<<"Evaluating classifier..."<<endl;
	Mat results;
	classifier.predict(evalData, &results);

	double errorRate = (double) countNonZero(groundTruth - results) / evalData.rows;
			;
	cout << "Error rate: " << errorRate << endl;*/

	//SVM Classifier
	CvSVM svm;

	cout << "Training classifier.... using SVM" << endl;
	svm.train(trainingData,labels);

	Mat resultsSVM;
	svm.predict(evalData,&resultsSVM);

	double errorRateSVM = (double) countNonZero(groundTruth - resultsSVM) / evalData.rows;

	cout << "Error rate: " << errorRateSVM << endl;



}
