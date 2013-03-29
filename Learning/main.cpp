#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ml/ml.hpp"


using namespace std;
using namespace cv;

int main()
{
    //Things to read files
    //char* directoryName = "../data/Resized/Index";
    char* directoryName = "/home/frederick/Code/OpenHGR/data/Resized/Paume";
    char* directoryNameIndex = "/home/frederick/Code/OpenHGR/data/Resized/Index";
    unsigned char isFile =0x8;
    DIR *directory;
    struct dirent *file;


    //Real Stuff
    Ptr<FeatureDetector> features = FeatureDetector::create("SIFT");
    Ptr<DescriptorExtractor> descriptor = DescriptorExtractor::create("SIFT");
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");

    //defining terms for bowkmeans trainer
    TermCriteria tc(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 10, 0.001);
    int dictionarySize = 100;
    int retries = 1;
    int flags = KMEANS_PP_CENTERS;
    BOWKMeansTrainer bowTrainer(dictionarySize, tc, retries, flags);

    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

    BOWImgDescriptorExtractor bowDE(descriptor, matcher);

    Mat features1, features2;
    Mat bowDescriptor, bowDescriptor2;

    Mat trainme(0, dictionarySize, CV_32FC1);

    Mat labels(0, 1, CV_32FC1);

    vector<KeyPoint> keypoints, keypoints2;



    SiftFeatureDetector detector;


    //On construit le dictionnaire
    directory = opendir(directoryName);


    while (file = readdir(directory))
    {
        if ( file->d_type == isFile)
        {
            string filepath = directoryName;
            filepath += "/";
            filepath += file->d_name;
            cout <<"Found a File : " << file->d_name << endl;
            cout << "File is at " << filepath << endl;

            Mat img = imread(filepath,CV_LOAD_IMAGE_GRAYSCALE);
            if (!img.data)
            {
                cout <<"Can't open file." << filepath << endl;
                continue;
            }


            detector.detect(img, keypoints);

            if (keypoints.size() > 0)
            {
                cout << "Detected keypoints :" << keypoints.size() << endl;

                descriptor->compute(img, keypoints, features1);

                float label = 1.0;
                labels.push_back(label);
                bowTrainer.add(features1);
                cout << "." << endl;
            }
        }
    }

    directory = opendir(directoryNameIndex);

    while (file = readdir(directory))
    {
        if ( file->d_type == isFile)
        {
            string filepath = directoryNameIndex;
            filepath += "/";
            filepath += file->d_name;
            cout <<"Found a File : " << file->d_name << endl;
            cout << "File is at " << filepath << endl;

            Mat img = imread(filepath,CV_LOAD_IMAGE_GRAYSCALE);
            if (!img.data)
            {
                cout <<"Can't open file." << filepath << endl;
                continue;
            }


            detector.detect(img, keypoints2);

            if (keypoints2.size() > 0)
            {
                cout << "Detected keypoints :" << keypoints.size() << endl;

                descriptor->compute(img, keypoints2, features2);
                bowDE.compute(img, keypoints2, bowDescriptor2);
                trainme.push_back(bowDescriptor2);
                float label = 2.0;
                labels.push_back(label);
                bowTrainer.add(features1);
                cout << "." << endl;
            }
        }
    }


    Mat dictionary = bowTrainer.cluster();
    bowDE.setVocabulary(dictionary);



    //On construit les descripteurs BOW
    directory = opendir(directoryName);


    while (file = readdir(directory))
    {
        if ( file->d_type == isFile)
        {
            string filepath = directoryName;
            filepath += "/";
            filepath += file->d_name;
            cout <<"Found a File : " << file->d_name << endl;
            cout << "File is at " << filepath << endl;

            Mat img = imread(filepath,CV_LOAD_IMAGE_GRAYSCALE);
            if (!img.data)
            {
                cout <<"Can't open file." << filepath << endl;
                continue;
            }


            detector.detect(img, keypoints);

            if (keypoints.size() > 0)
            {
                cout << "Detected keypoints :" << keypoints.size() << endl;

                descriptor->compute(img, keypoints, features1);



                bowDE.compute(img, keypoints, bowDescriptor);
                trainme.push_back(bowDescriptor);
                cout << "." << endl;
            }
        }
    }


     directory = opendir(directoryNameIndex);

    while (file = readdir(directory))
    {
        if ( file->d_type == isFile)
        {
            string filepath = directoryNameIndex;
            filepath += "/";
            filepath += file->d_name;
            cout <<"Found a File : " << file->d_name << endl;
            cout << "File is at " << filepath << endl;

            Mat img = imread(filepath,CV_LOAD_IMAGE_GRAYSCALE);
            if (!img.data)
            {
                cout <<"Can't open file." << filepath << endl;
                continue;
            }


            detector.detect(img, keypoints2);

            if (keypoints2.size() > 0)
            {
                cout << "Detected keypoints :" << keypoints.size() << endl;

                descriptor->compute(img, keypoints2, features2);
                bowDE.compute(img, keypoints2, bowDescriptor2);
                trainme.push_back(bowDescriptor2);
                cout << "." << endl;
            }
        }
    }

    cout << "Made it ! " << endl;

    //Mat dictionary = bowTrainer.cluster();
    //bowDE.setVocabulary(dictionary);

    CvSVM SVM;
    SVM.train(trainme,labels);

    Mat tryme(0, dictionarySize, CV_32FC1);
    Mat tryDescriptor;
    Mat img3 = imread("/home/frederick/Code/OpenHGR/data/Resized/Index/Index-1.1_50.pgm", 0);
    vector<KeyPoint> keypoints3;
    features->detect(img3, keypoints3);
    bowDE.compute(img3, keypoints3, tryDescriptor);
    tryme.push_back(tryDescriptor);

    cout<<SVM.predict(tryme)<<endl;

    /*Mat dictionary = bowTrainer.cluster();
    bowDE.setVocabulary(dictionary);

    CvSVM SVM;
    SVM.train(trainme,labels);

    Mat tryme(0, dictionarySize, CV_32FC1);
    Mat tryDescriptor;
    Mat img3 = imread("c:\\Users\\Elvan\\Desktop\\frame_0118.jpg", 0);
    vector<KeyPoint> keypoints3;
    features->detect(img3, keypoints3);
    bowDE.compute(img3, keypoints3, tryDescriptor);
    tryme.push_back(tryDescriptor);

    cout<<SVM.predict(tryme)<<endl;*/



    /*Ptr<FeatureDetector> features = FeatureDetector::create("SIFT");
    Ptr<DescriptorExtractor> descriptor = DescriptorExtractor::create("SIFT");
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");

    //defining terms for bowkmeans trainer
    TermCriteria tc(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 10, 0.001);
    int dictionarySize = 100;
    int retries = 1;
    int flags = KMEANS_PP_CENTERS;
    BOWKMeansTrainer bowTrainer(dictionarySize, tc, retries, flags);

    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

    BOWImgDescriptorExtractor bowDE(descriptor, matcher);

    Mat features1, features2;
    Mat bowDescriptor, bowDescriptor2;

    Mat trainme(0, dictionarySize, CV_32FC1);
    Mat labels(0, 1, CV_32FC1);

    while (dirp = readdir( dp ))
    {

        filepath = dir + "/" + dirp->d_name;
        // If the file is a directory (or is in some way invalid) we'll skip it
        if (stat( filepath.c_str(), &filestat )) continue;
        if (S_ISDIR( filestat.st_mode ))         continue;

        Mat img = imread(filepath);
        if (!img.data) {
            cout <<"Can't open file." << endl;
            continue;
        }
        features->detect(img, keypoints);
        descriptor->compute(img, keypoints, features1);
        bowDE.compute(img, keypoints, bowDescriptor);
        trainme.push_back(bowDescriptor);
        float label = 1.0;
        labels.push_back(label);
        bowTrainer.add(features1);
        cout << "." << endl;
    }

    while (dirp2 = readdir( dp2 ))
    {
        filepath2 = dir2 + "/" + dirp2->d_name;
        // If the file is a directory (or is in some way invalid) we'll skip it
        if (stat( filepath2.c_str(), &filestat2 )) continue;
        if (S_ISDIR( filestat2.st_mode ))         continue;

        Mat img2 = imread(filepath2);
        if (!img2.data) {
            cout <<"Can't open file." << endl;
            continue;
        }
        features->detect(img2, keypoints2);
        descriptor->compute(img2, keypoints2, features2);
        bowDE.compute(img2, keypoints2, bowDescriptor2);
        trainme.push_back(bowDescriptor2);
        float label = 0.0;
        labels.push_back(label);
        bowTrainer.add(features2);
        cout << "." << endl;
    }




Mat dictionary = bowTrainer.cluster();
bowDE.setVocabulary(dictionary);

CvSVM SVM;
SVM.train(trainme,labels);

Mat tryme(0, dictionarySize, CV_32FC1);
Mat tryDescriptor;
Mat img3 = imread("c:\\Users\\Elvan\\Desktop\\frame_0118.jpg", 0);
vector<KeyPoint> keypoints3;
features->detect(img3, keypoints3);
bowDE.compute(img3, keypoints3, tryDescriptor);
tryme.push_back(tryDescriptor);

cout<<SVM.predict(tryme)<<endl;*/
}
