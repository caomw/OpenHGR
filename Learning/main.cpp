#include <fstream>
#include <iostream>
#include "AlgorithmTester.h"

using namespace std;

int main()
{

    for (int i=550; i <= 2500; i+=25)
    {
        for (int j=0; j<3;j++)
        {
            ofstream out ( "/home/frederick/Code/OpenHGR/results2.csv", ios::app );

            cout << "Testing K = " << i << endl;
            AlgorithmTester* test;
            vector<string> trainingFolders;
            string testFolder;

            if (j ==0)
            {
                trainingFolders.push_back("/home/frederick/Code/OpenHGR/data/Dataset2/1.DataSet");
                trainingFolders.push_back("/home/frederick/Code/OpenHGR/data/Dataset2/2.DataSet");
                testFolder = "/home/frederick/Code/OpenHGR/data/Dataset2/3.DataSet";
                out << 1 << ";" << 2 << ";" << 3 << ";";
            }
            else if (j==1)
            {
                trainingFolders.push_back("/home/frederick/Code/OpenHGR/data/Dataset2/1.DataSet");
                trainingFolders.push_back("/home/frederick/Code/OpenHGR/data/Dataset2/3.DataSet");
                testFolder = "/home/frederick/Code/OpenHGR/data/Dataset2/2.DataSet";
                out << 1 << ";" << 3 << ";" << 2 << ";";
            }
            else
            {
                trainingFolders.push_back("/home/frederick/Code/OpenHGR/data/Dataset2/2.DataSet");
                trainingFolders.push_back("/home/frederick/Code/OpenHGR/data/Dataset2/3.DataSet");
                testFolder = "/home/frederick/Code/OpenHGR/data/Dataset2/1.DataSet";
                out << 2 << ";" << 3 << ";" << 1 << ";";
            }

            test = new AlgorithmTester(SURF_Detector,SURF_Extractor,FlannBased,SVM_Model,i,trainingFolders,testFolder);

            test->run();



            //Writing stats
            out << i << ";SURF;SURF;FlannBased;SVM;" << test->cptrKeypointsTraining << ";" << test->cptrImagesTraining/test->cptrImagesTraining << ";"
                << test->timeElapsedLearning << ";" << test->timeElapsedTesting << ";" << test->errorRate << ";";

            for (int k=0; k < test->NBR_GESTURE; k++)
            {
                for (int l=0;  l<= test->NBR_GESTURE; l++)
                {
                    out << test->gesturesCptr[k][l] << ";";
                }
            }
            out << endl;
            out.close();


            delete test;
        }
    }


    cout << "Fin" << endl;


    //DescriptorDetectorType descriptorDetectorType, DescriptorExtractorType descriptorExtractorType, DescriptorMatcherType descriptorMatcherType,
                        //StatisticalModelType statisticalModelType, int kValue, vector<string> _trainingFolders, string _testFolder):

}
