#include <iostream>
#include <stdio.h>
#include <unistd.h>

// OpenCV includes
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// Filter/Filter factories includes
#include <AbstractHandThresholder.h>
#include <AbstractHandDetector.h>
#include <AbstractGestureRecognizer.h>
#include <HandThresholderFactory.h>
#include <HandDetectorFactory.h>
#include <GestureRecognizerFactory.h>

using namespace std;
using namespace cv;

int main()
{
    char* benchmark_file = "../data/Benchmarks/2.avi";
    //char* benchmark_file = "../data/benchmark1.avi";
    //char* benchmark_file = "../data/benchmark3.avi";

    CvCapture* capture;
    Mat frame, f, thresholdFrame;

    // Filter factories
    HandThresholderFactory htf;
    HandDetectorFactory hdf;
    GestureRecognizerFactory grf;

    // Filters
    AbstractHandThresholder* handThresholder = htf.createInstance(HAAR_HSV);
    AbstractHandDetector* handDetector = hdf.createInstance(CONTOUR_COMPARISON);
    AbstractGestureRecognizer* gestureRecognizer = grf.createInstance(SIFT);

    // Debugging
    handThresholder->setDebug(1);
    handDetector->setDebug(1);
    //gestureRecognizer->setDebug(1);

    int video = 1;

    if ( video )
    {
        capture = cvCaptureFromFile(benchmark_file);
    }
    else
    {
        //capture = cvCaptureFromCAM( -1 );
        capture = cvCreateCameraCapture(0);
        cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 640 );
        cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 480 );
    }

    if ( capture )
    {
        while( true )
        {
            f = cvQueryFrame( capture );
            flip(f,frame,1);

            if( !frame.empty() )
            {
                thresholdFrame = handThresholder->thresholdHand(frame);
                Rect handRect = handDetector->detectHand(thresholdFrame);
                string gesture = gestureRecognizer->recognizeGesture(frame, handRect);

                //cout << gesture << endl;

                rectangle(frame, Point(handRect.x, handRect.y), Point(handRect.x+handRect.width, handRect.y+handRect.height), Scalar(0,0,255), 2, 8 );
                imshow("Main", frame);
            }
            else
            {
                if ( video )
                {
                    capture = cvCaptureFromFile(benchmark_file);
                }
                else
                {
                    break;
                }
            }

            int c = waitKey(10);
            if( (char)c == 'c' ) { break; }
            //sleep(1);
        }
    }

    return 0;
}
