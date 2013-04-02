#include "ConvexHullComparisonHandDetector.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <dirent.h>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

ConvexHullComparisonHandDetector::ConvexHullComparisonHandDetector()
{
    //ctor
}

ConvexHullComparisonHandDetector::~ConvexHullComparisonHandDetector()
{
    //dtor
}

Rect ConvexHullComparisonHandDetector::detectHand ( Mat input )
{
    Mat gray;
    int thresh = 100;
    int max_tresh = 255;
    RNG rng(12345);
    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Rect tmp(0,0,0,0);


     int contourIndex = 0;
     vector<Point> points;

    //Convert to gray and little blur
    //cvtColor(input,gray,CV_BGR2GRAY);
    //blur(gray,gray,Size(3,3));

    //Detected Edges using Threshold
    threshold(input,threshold_output,thresh,255,THRESH_BINARY);

    //Finding contours
    findContours(threshold_output,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<vector<int> > hullsI(contours.size());
     vector<vector<Point> > hullsP(contours.size());
     vector<vector<Vec4i> > defects(contours.size());

    for (uint i=0; i<contours.size();i++)
     {
         convexHull(contours[i],hullsI[i],false);
         convexHull(contours[i],hullsP[i],false,true);
         if (hullsI[i].size()>3)
            convexityDefects(contours[i],hullsI[i],defects[i]);

        if (i != 0 && defects[i].size() > defects[contourIndex].size())
            contourIndex = i;
     }

     //for (uint i=0; i< contours.size();i++)
     //{
int i = contourIndex;
         //http://docs.opencv.org/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html
         for (uint j=0; j < defects[i].size();j++)
         {
             Vec4i tmp = defects[i][j]; //{startIndex,endIndex,farIndex,depth}
             Point start = contours[i][tmp[0]];
             Point end = contours[i][tmp[1]];
             Point far = contours[i][tmp[2]];
             float depth = tmp[3]/256.0;

             if (depth > 50)
             {

                 points.push_back(far);
                //line(drawing,start,far,Scalar(0,255,0));
                //line(drawing,end,far,Scalar(0,255,0));
                //circle(drawing,far,3,Scalar(255,0,0));
                //circle(drawing,end,3,Scalar(0,0,255));
                //cptrDoigts++;
             }
             else
             {
                 //line(drawing,start,far,Scalar(0,255,0));
                 //line(drawing,end,far,Scalar(0,255,0));
             }

         }

         if (points.size() > 0)
         {
             Point2f center;
             float radius;
             minEnclosingCircle(points,center,radius);
             tmp = Rect(center.x-(50),center.y+(50),100,100);
             return tmp;

             //RotatedRect rect = minAreaRect(points);

             //ellipse(drawing,rect,Scalar(0,255,255),3);
             //circle(drawing,Point((int)center.x,(int)center.y),(int)radius,Scalar(0,255,0),3);
         }
         else
            return tmp;
     //}




}
/*{
    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    int biggestContour = 0;
    Rect handRect(0,0,0,0);

    Canny( input, canny_output, 100, 100*2, 3 );
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    for ( int i = 0; i < contours.size(); i ++ )
            for ( int j = 0; j < templates.size(); j ++ )
            {
                double match = matchShapes ( contours[i], templates[j], CV_CONTOURS_MATCH_I3, 0 );
                if ( match < shapeMatchThreshold && match > 0 )
                {
                    Rect r = boundingRect(contours[i]);
                    if ( r.width*r.height >= biggestContour )
                    {
                        biggestContour = r.width*r.height;
                        handRect = r;
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
                double match = matchShapes ( contours[i], templates[j], CV_CONTOURS_MATCH_I3, 0 );
                if ( match < 2 && match > 0 )
                {
                    cout << "match : " << match << endl;
                    Rect r = boundingRect(contours[i]);
                    rectangle(drawing, Point(r.x, r.y), Point(r.x+r.width, r.y+r.height), Scalar(255), 2, 8 );
                }
            }


        rectangle(drawing, Point(handRect.x, handRect.y), Point(handRect.x+handRect.width, handRect.y+handRect.height), Scalar(0,0,255), 2, 8 );

        imshow("ContourComparisonHandDetector", drawing);
    }

    return handRect;
}*/
