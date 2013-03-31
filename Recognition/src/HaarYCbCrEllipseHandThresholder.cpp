#include "HaarYCbCrEllipseHandThresholder.h"
#include <iostream>
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

HaarYCbCrEllipseHandThresholder::HaarYCbCrEllipseHandThresholder()
{
    if( !face_cascade.load( face_cascade_name ) )
        printf("--(!)Error loading\n");

    skinCrCbHist = Mat::zeros(Size(256, 256), CV_8UC1);
    ellipse(skinCrCbHist, Point(113, 155.6), Size(23.4, 15.2), 43.0, 0.0, 360.0, Scalar(255, 255, 255), -1);
}

cv::Mat HaarYCbCrEllipseHandThresholder::thresholdHand ( cv::Mat input )
{
    Mat bw = thresholdYCbCr ( substractFace(input) );
    Mat bw2;
    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    //morphologyEx( bw, bw2, MORPH_CLOSE, element );
    //bw = bw2;

    if ( isDebug())
        imshow ( "HaarYCbCrEllipseHandThresholder", bw );

    return bw;
}

cv::Mat HaarYCbCrEllipseHandThresholder::thresholdYCbCr ( cv::Mat input )
{
    Mat ycbcr;
    Mat output = Mat::zeros(input.size(), CV_8UC1);

    cvtColor(input, ycbcr, CV_BGR2YCrCb, 0);

    for(int row = 0; row < input.rows; ++row)
        for(int col = 0; col < input.cols; ++col)
        {
             Vec3b pt = ycbcr.at<Vec3b>(row,col);
             if(pt[0] > 128)
             if ((skinCrCbHist.at<uchar>(pt[1], pt[2]) > 0))
                  output.at<uchar>(row,col) = 255;
        }

    return output;
}

Mat HaarYCbCrEllipseHandThresholder::substractFace ( Mat input )
{
    std::vector<Rect> faces;
    Mat output = input.clone();

    Mat frame_gray;

    cvtColor( input, frame_gray, CV_BGR2GRAY );

    // Face detection
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    // Face substraction
    for( int i = 0; i < faces.size(); i++ )
    {
        // Circle
        Point center ( faces[i].x + faces[i].width / 2.0, faces[i].y + faces[i].height / 2.0);
        circle( output, center, faces[i].width / 1.4, Scalar( 0, 0, 0), -1, 8 );
    }

    return output;
}


HaarYCbCrEllipseHandThresholder::~HaarYCbCrEllipseHandThresholder()
{
    //dtor
}
