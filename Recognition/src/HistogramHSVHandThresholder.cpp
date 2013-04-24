#include "HistogramHSVHandThresholder.h"
#include <iostream>
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

HistogramHSVHandThresholder::HistogramHSVHandThresholder()
{
    if( !face_cascade.load( face_cascade_name ) )
        printf("--(!)Error loading\n");
}

cv::Mat HistogramHSVHandThresholder::thresholdHand ( cv::Mat input )
{
    std::vector<Rect> faces;
    Mat frame_gray, bw, bw2, inHSV;
    Mat out = input;

    cvtColor( input, inHSV, CV_BGR2HSV );
    cvtColor( input, frame_gray, CV_BGR2GRAY );

    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    if ( faces.size() > 0 )
    {
        Rect faceRect = faces [0];
        Mat face = input(faceRect);
        Mat faceHSV, histHue, histSat;
        MatND histogram;
        //int h_bins = 32, s_bins = 30;
        int h_bins = 30, s_bins = 32;
        int min_hue,max_hue,min_sat,max_sat;
        int channels[] = { 0, 1 };
        int histSize[] = { h_bins, s_bins };
        int histHueSize[] = {32};
        int histSatSize[] = {30};
        float h_ranges[] = { 0, 256 };
        float s_ranges[] = { 0, 180 };
        double maxValue, maxIndex;
        const float* ranges[] = { h_ranges, s_ranges };
        const float* hh_ranges[] = {h_ranges};
        const float* ss_ranges[] = {s_ranges};
        vector<Mat> splitHSV;

        cvtColor( face, faceHSV, CV_BGR2HSV );

        calcHist( &faceHSV, 1, channels, Mat(), histogram, 2, histSize, ranges, true, false );

///////////////////////////////////////////////////////////////

        double maxVal = 0;
        Point maxLoc;
        minMaxLoc(histogram, 0, &maxVal, 0, &maxLoc);
        cout << maxLoc.x << ":" << maxLoc.y << ":" << endl;

        int scale = 10;
        Mat histImg = Mat::zeros(s_bins*scale, h_bins*10, CV_8UC3);

    for( int h = 0; h < h_bins; h++ )
        for( int s = 0; s < s_bins; s++ )
        {
            float binVal = histogram.at<float>(h, s);
            int intensity = cvRound(binVal*255/maxVal);
            rectangle( histImg, Point(h*scale, s*scale),
                        Point( (h+1)*scale - 1, (s+1)*scale - 1),
                        Scalar::all(intensity),
                        CV_FILLED );
        }

        imshow("abc", histImg);
////////////////////////////////////////////////////////////////
        split(faceHSV,splitHSV);
        calcHist(&splitHSV[0],1,0,Mat(),histHue,1,histHueSize,hh_ranges,true,false);
        minMaxLoc(histHue,0,&maxValue,0,0);

        for (int i = 0; i < h_bins; i++)
        {
            float binval = histHue.at<float>(i);
            if (binval == maxValue)
                hue_skin_min_value = 8*i;
        }

        calcHist(&splitHSV[1],1,0,Mat(),histSat,1,histSatSize,ss_ranges,true,false);
        minMaxLoc(histSat,0,&maxValue,0,0);

        for (int i=0; i< h_bins; i++)
        {
            float binval = histSat.at<float>(i);
            if (binval == maxValue)
                sat_skin_min_value = 6*i;
        }

        min_hue = ( hue_skin_min_value - 8  < 0 ) ? 0    : hue_skin_min_value - 8;
        max_hue = ( hue_skin_min_value + 16 > 256) ? 256 : hue_skin_min_value + 16;
        min_sat = ( sat_skin_min_value - 24 < 0) ? 0     : sat_skin_min_value - 24;
        max_sat = ( sat_skin_min_value + 30 > 180) ? 180 : sat_skin_min_value + 30;

        minHSV = Scalar(min_hue,50,0);
        maxHSV = Scalar(max_hue,190,255);

        Point center ( faceRect.x + faceRect.width / 2.0, faceRect.y + faceRect.height / 2.0);
        circle( inHSV, center, faceRect.width / 1.4, Scalar( 0, 0, 0), -1, 8 );
    }

    inRange(inHSV, minHSV, maxHSV, bw);

    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    morphologyEx( bw, bw2, MORPH_CLOSE, element );
    bw = bw2;

    if (isDebug())
        imshow("HistogramHSVHandThresholder", bw);

    return bw;
}

HistogramHSVHandThresholder::~HistogramHSVHandThresholder()
{
    //dtor
}
