#include "ContourComparisonHandDetector.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <dirent.h>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

ContourComparisonHandDetector::ContourComparisonHandDetector()
{
    loadTemplates ();
}

ContourComparisonHandDetector::~ContourComparisonHandDetector()
{
    //dtor
}

cv::Rect ContourComparisonHandDetector::detectHand ( cv::Mat input )
{
    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    Canny( input, canny_output, 100, 100*2, 3 );
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );




    if ( isDebug())
    {
        Mat drawing = Mat::zeros( input.size(), CV_8UC3 );
        for( int i = 0; i< contours.size(); i++ )
        {
           Scalar color = Scalar( rand()%255, rand()%255, rand()%255 );
           drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
        }

        for ( int i = 0; i < contours.size(); i ++ )
        for ( int j = 0; j < templates.size(); j ++ )
        {
            double match = cv::matchShapes ( contours[i], templates[j], CV_CONTOURS_MATCH_I3, 0 );
            if ( match < 0.4 && match > 0 )
            {
                cout << match << ":" << boundingRect(contours[i]).width << endl;
                Rect rect = boundingRect(contours[i]);
                rectangle(drawing, Point(rect.x, rect.y), Point(rect.x+rect.width, rect.y+rect.height), Scalar(255), 2, 8 );
            }
        }





        imshow("Drawing", drawing);
    }

    return cv::Rect(0,0,1,1);
}

void ContourComparisonHandDetector::loadTemplates ()
{
    DIR* directory = opendir(templates_path.c_str());
    struct dirent *file;
    unsigned char isFile =0x8;

    while (file = readdir(directory))
    {
        if ( file->d_type == isFile)
        {
            string filepath = templates_path + "/" + file->d_name;
            loadTemplate ( filepath );
        }
    }
}

void ContourComparisonHandDetector::loadTemplate( string file )
{
    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    Mat image = imread(file, CV_LOAD_IMAGE_GRAYSCALE);

    Canny( image, canny_output, 100, 100*2, 3 );
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    int indexBiggestContour = -1;
    int sizeBiggestContour = 0;
    for (int i = 0; i < contours.size(); i++)
    {
        if(contours[i].size() > sizeBiggestContour)
        {
            sizeBiggestContour = contours[i].size();
            indexBiggestContour = i;
        }
    }

    if ( indexBiggestContour != -1 )
        templates.push_back(contours[indexBiggestContour]);
}
