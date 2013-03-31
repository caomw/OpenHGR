#include "HaarAdaptiveHandThresholder.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"

using namespace std;
using namespace cv;

HaarAdaptiveHandThresholder::HaarAdaptiveHandThresholder()
{
}

cv::Mat HaarAdaptiveHandThresholder::thresholdHand ( cv::Mat input )
{
    IplImage *img = new IplImage(input);
    if ( filterMask == NULL )
        filterMask = cvCreateImage( cvSize(img->width, img->height), IPL_DEPTH_8U, 1);

    CvAdaptiveSkinDetector filter(1, CvAdaptiveSkinDetector::MORPHING_METHOD_ERODE_DILATE);
    filter.process(img, filterMask);















    Mat output(filterMask);

    if ( isDebug() )
        imshow ( "HaarAdaptiveHandThresholder", output );

    return output;
}

HaarAdaptiveHandThresholder::~HaarAdaptiveHandThresholder()
{
    if ( filterMask != NULL )
        cvReleaseImage ( &filterMask );
}

void HaarAdaptiveHandThresholder::displayBuffer(IplImage *rgbDestImage, IplImage *buffer, int rValue, int gValue, int bValue)
{
    int x, y, nWidth, nHeight;
    double destX, destY, dx, dy;
    uchar c;
    unsigned char *pSrc;

    nWidth = buffer->width;
    nHeight = buffer->height;

    dx = double(rgbDestImage->width)/double(nWidth);
    dy = double(rgbDestImage->height)/double(nHeight);

    destX = 0;
    for (x = 0; x < nWidth; x++)
    {
        destY = 0;
        for (y = 0; y < nHeight; y++)
        {
            c = ((uchar*)(buffer->imageData + buffer->widthStep*y))[x];

            if (c)
            {
                pSrc = (unsigned char *)rgbDestImage->imageData + rgbDestImage->widthStep*int(destY) + (int(destX)*rgbDestImage->nChannels);
                //ASD_RGB_SET_PIXEL(pSrc, rValue, gValue, bValue);
            }
            destY += dy;
        }
        destY = 0;
        destX += dx;
    }
};
