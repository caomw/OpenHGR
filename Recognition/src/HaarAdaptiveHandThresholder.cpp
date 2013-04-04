#include "HaarAdaptiveHandThresholder.h"
#include <iostream>
#include <stdio.h>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"

using namespace std;
using namespace cv;

HaarAdaptiveHandThresholder::HaarAdaptiveHandThresholder()
{
    if( !face_cascade.load( face_cascade_name ) )
        cout << "--(!)Error loading\n" <<endl;
}

cv::Mat HaarAdaptiveHandThresholder::thresholdHand ( cv::Mat input )
{
    Mat frame = substractFace (input);
    Mat bw;
    IplImage *img = new IplImage(frame);
    if ( filterMask == NULL )
        filterMask = cvCreateImage( cvSize(img->width, img->height), IPL_DEPTH_8U, 1);

    CvAdaptiveSkinDetector filter(1, CvAdaptiveSkinDetector::MORPHING_METHOD_ERODE_DILATE);
    filter.process(img, filterMask);

    Mat mat(filterMask);
    inRange(mat, Scalar(1,1,1), Scalar(255,255,255), frame);

    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    morphologyEx( frame, bw, MORPH_CLOSE, element );

    if ( isDebug() )
        imshow ( "HaarAdaptiveHandThresholder", bw);

    return frame;
}

HaarAdaptiveHandThresholder::~HaarAdaptiveHandThresholder()
{
    if ( filterMask != NULL )
        cvReleaseImage ( &filterMask );
}

Mat HaarAdaptiveHandThresholder::substractFace ( Mat input )
{
    std::vector<Rect> faces;
    Mat output = input.clone();

    Mat frame_gray;

    cvtColor( input, frame_gray, CV_BGR2GRAY );

    // Face detection
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    cout << faces.size();
    // Face substraction
    for( int i = 0; i < faces.size(); i++ )
    {
        // Circle
        Point center ( faces[i].x + faces[i].width / 2.0, faces[i].y + faces[i].height / 2.0);
        circle( output, center, faces[i].width / 1.4, Scalar( 0, 0, 0), -1, 8 );
    }

    return output;
}
