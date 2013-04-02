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
    int biggestContour = 0;
    Rect handRect(0,0,0,0);

    Canny( input, canny_output, 100, 100*2, 3 );
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    for ( int i = 0; i < contours.size(); i ++ )
    {
        Rect r = boundingRect(contours[i]);
        r.height = r.width*widthHeightRadio;
        vector<Point> newContour = cropContour(contours[i], r );

        for ( int j = 0; j < templates.size(); j ++ )
        {
            double match = matchShapes ( newContour, templates[j], CV_CONTOURS_MATCH_I3, 0 );

            if ( match < shapeMatchThreshold && match > 0 )
            {
                r = boundingRect(newContour);
                if ( r.width*r.height >= biggestContour )
                {
                    cout << "match : " << match << endl;
                    biggestContour = r.width*r.height;
                    handRect = r;
                }
            }
        }
    }

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
                Rect r = boundingRect(contours[i]);
                r.height = r.width * widthHeightRadio;
                vector<Point> newContour = cropContour(contours[i], r );

                double match = matchShapes ( newContour, templates[j], CV_CONTOURS_MATCH_I3, 0 );
                if ( match < shapeMatchThreshold && match > 0 )
                {
                    r = boundingRect(newContour);
                    rectangle(drawing, Point(r.x, r.y), Point(r.x+r.width, r.y+r.height), Scalar(255), 2, 8 );
                }
            }


        rectangle(drawing, Point(handRect.x, handRect.y), Point(handRect.x+handRect.width, handRect.y+handRect.height), Scalar(0,0,255), 2, 8 );

        imshow("ContourComparisonHandDetector", drawing);
    }

    return handRect;
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

vector<Point> ContourComparisonHandDetector::cropContour ( vector<Point> contour, Rect rect )
{
    vector<Point>  newVector;

    for ( int i = 0; i < contour.size(); i ++ )
        if ( contour[i].inside(rect))
            newVector.push_back(contour[i]);

    return newVector;
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
