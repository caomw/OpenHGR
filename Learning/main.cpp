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
    char* directoryName = "/home/frederick/Code/OpenHGR/data/Fred/Paume/Cropped";
    char* directoryNameIndex = "/home/frederick/Code/OpenHGR/data/Fred/Index/Cropped";
    char* directoryNameTest = "/home/frederick/Code/OpenHGR/data/Cropped/Paume";
    String names[2] = {"Paume","Index"};
    int expected = 1;
    int correct = 0;
    int tried = 0;


    unsigned char isFile =0x8;
    DIR *directory;
    struct dirent *file;
    long nbKeypoints = 0;
    long cptr = 0;


    //Real Stuff
    Ptr<FeatureDetector> features = FeatureDetector::create("SURF");
    Ptr<DescriptorExtractor> descriptor = DescriptorExtractor::create("SURF");
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");

    //defining terms for bowkmeans trainer
    TermCriteria tc(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 100, 0.001);
    int dictionarySize = 15;
    int retries = 20;
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



    SurfFeatureDetector detector;


    //On construit le dictionnaire
    directory = opendir(directoryName);



    while (file = readdir(directory))
    {
        if ( file->d_type == isFile)
        {
            string filepath = directoryName;
            filepath += "/";
            filepath += file->d_name;
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

                cout << "Detected features :" << features1.size() << endl;
                cout << "Detected features height :" << features1.size().height << endl;
                nbKeypoints = features1.size().height;
                cptr++;



                bowTrainer.add(features1);
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
                cout << "Detected keypoints :" << keypoints2.size() << endl;

                descriptor->compute(img, keypoints2, features2);
                cout << "Detected features :" << features2.size() << endl;
                cout << "Detected features height :" << features2.size().height << endl;
                nbKeypoints += features2.size().height;
                cptr++;



                bowTrainer.add(features2);
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
            //cout <<"Found a File : " << file->d_name << endl;
            //cout << "File is at " << filepath << endl;

            Mat img = imread(filepath,CV_LOAD_IMAGE_GRAYSCALE);
            if (!img.data)
            {
                cout <<"Can't open file." << filepath << endl;
                continue;
            }


            detector.detect(img, keypoints);

            if (keypoints.size() > 0)
            {
                //cout << "Detected keypoints :" << keypoints.size() << endl;

                descriptor->compute(img, keypoints, features1);



                bowDE.compute(img, keypoints, bowDescriptor);
                trainme.push_back(bowDescriptor);

                 float label = 1.0;
                labels.push_back(label);
                //cout << "." << endl;
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
            //cout <<"Found a File : " << file->d_name << endl;
            //cout << "File is at " << filepath << endl;

            Mat img = imread(filepath,CV_LOAD_IMAGE_GRAYSCALE);
            if (!img.data)
            {
                //cout <<"Can't open file." << filepath << endl;
                continue;
            }


            detector.detect(img, keypoints2);

            if (keypoints2.size() > 0)
            {
                //cout << "Detected keypoints :" << keypoints2.size() << endl;

                descriptor->compute(img, keypoints2, features2);
                bowDE.compute(img, keypoints2, bowDescriptor2);
                trainme.push_back(bowDescriptor2);

                float label = 2.0;
                labels.push_back(label);
                //cout << "." << endl;
            }
        }
    }

    //cout << "Made it ! " << endl;

    //Mat dictionary = bowTrainer.cluster();
    //bowDE.setVocabulary(dictionary);

    CvSVM SVM;
    SVM.train(trainme,labels);



    directory = opendir(directoryNameTest);


    while (file = readdir(directory))
    {
        if ( file->d_type == isFile)
        {
            string filepath = directoryNameTest;
            filepath += "/";
            filepath += file->d_name;
            cout <<"Found a File : " << file->d_name << endl;
            //cout << "File is at " << filepath << endl;

            Mat img3 = imread(filepath,CV_LOAD_IMAGE_GRAYSCALE);
            Mat tryme(0, dictionarySize, CV_32FC1);
            Mat tryDescriptor;
            if (!img3.data)
            {
                //cout <<"Can't open file." << filepath << endl;
                continue;
            }


            vector<KeyPoint> keypoints3;


            features->detect(img3, keypoints3);
            bowDE.compute(img3, keypoints3, tryDescriptor);
            tryme.push_back(tryDescriptor);

            int guess = (int)SVM.predict(tryme)-1;

            cout<<names[guess]<<endl;
            tried++;
            if (guess == expected)
                correct++;
        }
    }

    /*CvCapture* capture;
    capture = cvCaptureFromFile("../data/Benchmark-Fred.avi");
    Mat f, frame;

     if ( capture )
    {
        while( true )
        {
            f = cvQueryFrame( capture );
            flip(f,frame,1);

              if( !frame.empty() )
            {
                Mat tryme(0, dictionarySize, CV_32FC1);
                Mat tryDescriptor;
                vector<KeyPoint> keypoints3;


                features->detect(frame, keypoints3);
                bowDE.compute(frame, keypoints3, tryDescriptor);
                tryme.push_back(tryDescriptor);

                int guess = (int)SVM.predict(tryme)-1;

                cout<<names[guess]<<endl;
                imshow("Main", frame);
            }
            else
            {
                 break;
            }

            int c = waitKey(10);
            if( (char)c == 'c' ) { break; }

        }
    }*/



    cout << "Correct :" << correct << "/" << tried << endl;
    cout << "Average features detected :" << nbKeypoints / cptr << endl;
    cout << "Cptr :" << cptr << endl;


}
