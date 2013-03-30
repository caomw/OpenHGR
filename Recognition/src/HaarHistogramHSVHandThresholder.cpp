#include "HaarHistogramHSVHandThresholder.h"
#include <iostream>
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

HaarHistogramHSVHandThresholder::HaarHistogramHSVHandThresholder()
{
     if( !face_cascade.load( face_cascade_name ) )
        printf("--(!)Error loading\n");
}

cv::Mat HaarHistogramHSVHandThresholder::thresholdHand ( cv::Mat input )
{
  std::vector<Rect> faces;
  Mat frame_gray, out, bw, bw2;

  out = input;
      Mat inHSV;
     cvtColor( input, inHSV, CV_BGR2HSV );

  cvtColor( input, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  // Détection des visages
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  if ( faces.size() > 0 )
  {
    int i = 0;

    //Test extraire la face
    Mat face = input(faces[i]);
    Mat facecpy;
    Mat faceHSV;
    face.copyTo(facecpy);

    //On travaille en HSV
    cvtColor( facecpy, faceHSV, CV_BGR2HSV );

    //On contruit l'histograme
    int h_bins = 32; int s_bins = 30;
    int histSize[] = { h_bins, s_bins };

    float h_ranges[] = { 0, 256 };
    float s_ranges[] = { 0, 180 };

    const float* ranges[] = { h_ranges, s_ranges };

    int channels[] = { 0, 1 };

    MatND histogramme;

    calcHist( &faceHSV, 1, channels, Mat(), histogramme, 2, histSize, ranges, true, false );
    double maxValue, maxIndex;

    vector<Mat> splitHSV;

    split(faceHSV,splitHSV);

    Mat histHue;
    const float* hh_ranges[] = {h_ranges};
    int histHueSize[] = {32};


    calcHist(&splitHSV[0],1,0,Mat(),histHue,1,histHueSize,hh_ranges,true,false);

    minMaxLoc(histHue,0,&maxValue,0,0);



    for (int i=0; i< h_bins; i++)
    {
        float binval = histHue.at<float>(i);
        if (binval == maxValue)
            {
                //cout << "Max Value is hue at index" << i << endl;
                hue_skin_min_value = 8*i;
            }
    }

    Mat histSat;
    const float* ss_ranges[] = {s_ranges};

    int histSatSize[] = {30};
    calcHist(&splitHSV[1],1,0,Mat(),histSat,1,histSatSize,ss_ranges,true,false);

    minMaxLoc(histSat,0,&maxValue,0,0);

    for (int i=0; i< h_bins; i++)
    {
        float binval = histSat.at<float>(i);
        if (binval == maxValue)
            {
                //cout << "Max Value is sat at index" << i << endl;
                sat_skin_min_value = 6*i;
            }
    }


    int min_hue,max_hue,min_sat,max_sat;

    if (hue_skin_min_value-8 <0)
        min_hue = 0;
    else
        min_hue = hue_skin_min_value-8;

    if (hue_skin_min_value+16 > 256)
        max_hue = 256;
    else
        max_hue = hue_skin_min_value + 16;

    if (sat_skin_min_value-24 <0)
        min_sat = 0;
    else
        min_sat = sat_skin_min_value-24;


    if (sat_skin_min_value+30 > 180)
        max_sat = 180;
    else
        max_sat = sat_skin_min_value+30;

    Mat bw;

    minHSV = Scalar(min_hue,50,0);
    maxHSV = Scalar(max_hue,190,255);

    Point center ( faces[i].x + faces[i].width / 2.0, faces[i].y + faces[i].height / 2.0);
    circle( inHSV, center, faces[i].width / 1.4, Scalar( 0, 0, 0), -1, 8 );
  }

   inRange(inHSV, minHSV, maxHSV, bw);

    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    morphologyEx( bw, bw2, MORPH_CLOSE, element );
    bw = bw2;

      if (isDebug())
        imshow("HaarHistogramHSVHandThresholder", bw);

  return bw;
}

HaarHistogramHSVHandThresholder::~HaarHistogramHSVHandThresholder()
{
    //dtor
}
