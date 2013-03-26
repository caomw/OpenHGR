#include <iostream>
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

String face_cascade_name = "haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
int hue_skin_min_value;
int sat_skin_min_value;

Mat substractFace ( Mat in )
{
  std::vector<Rect> faces;
  Mat frame_gray, out;

  out = in;

  cvtColor( in, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  // Détection des visages
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  for( int i = 0; i < faces.size(); i++ )
  {
    // Circle
    Point center ( faces[i].x + faces[i].width / 2.0, faces[i].y + faces[i].height / 2.0);
    circle( out, center, faces[i].width / 1.6, Scalar( 0, 0, 0), -1, 8 );
  }

  return out;
}


Mat substractFace2 ( Mat in )
{
  std::vector<Rect> faces;
  Mat frame_gray, out;

  out = in;

  cvtColor( in, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  // Détection des visages
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );


  for( int i = 0; i < faces.size(); i++ )
  {
    //Test extraire la face
    Mat face = in(faces[i]);
    Mat facecpy;
    Mat faceHSV;
    face.copyTo(facecpy);

    //On travail en HSV
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

    imshow("Hue", splitHSV[0]);
    imshow("Saturation",splitHSV[1]);
    imshow("Value", splitHSV[2]);


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
                cout << "Max Value is hue at index" << i << endl;
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
                cout << "Max Value is sat at index" << i << endl;
                sat_skin_min_value = 6*i;
            }
    }

    Mat inHSV;
     cvtColor( in, inHSV, CV_BGR2HSV );

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
//For the Correlation and Intersection methods, the higher the metric, the more accurate the match. As we can see, the match base-base is the highest of all as expected. Also we can observe that the match base-half is the second best match (as we predicted). For the other two metrics, the less the result, the better the match. We can observe that the matches between the test 1 and test 2 with respect to the base are worse, which again, was expected.


    if (sat_skin_min_value+30 > 180)
        max_sat = 180;
    else
        max_sat = sat_skin_min_value+30;

    Mat bw;
    inRange(inHSV,Scalar(min_hue,min_sat,0),Scalar(max_hue,max_sat,255),bw);

    imshow("BW", bw);






    imshow( "ta face", facecpy );



    // Rectangle
    Point center ( faces[i].x + faces[i].width / 2.0, faces[i].y + faces[i].height / 2.0);
    circle( out, center, faces[i].width / 1.6, Scalar( 0, 0, 0), -1, 8 );
  }

  return out;
}

int findBiggestContour(vector<vector<Point> > contours){
    int indexOfBiggestContour = -1;
    int sizeOfBiggestContour = 0;
    for (int i = 0; i < contours.size(); i++){
        if(contours[i].size() > sizeOfBiggestContour){
            sizeOfBiggestContour = contours[i].size();
            indexOfBiggestContour = i;
        }
    }
    return indexOfBiggestContour;
}



int main()
{
   CvCapture* capture;
   Mat frame, f, hsv, bw;

 //-- 1. Load the cascades
   if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

   int video = 1;

   if ( video )
   {
       capture = cvCaptureFromFile("../data/benchmark1.avi");
   }
   else
   {
          //-- Read the video stream
       capture = cvCaptureFromCAM( -1 );
   //capture = cvCreateCameraCapture(1);
   //cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 640 );
   //cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 480 );
   }




    if( capture )
   {
     while( true )
     {
       f = cvQueryFrame( capture );
       flip(f,frame,1);
       //-- 3. Apply the classifier to the frame
       if( !frame.empty() )
       {
           frame = substractFace2(frame);
           imshow(" Test ",frame);
           /*cvtColor ( frame, hsv, CV_BGR2HSV);
           blur( hsv, hsv, Size(3,3) );


           inRange(hsv, Scalar(0,  50, 0), Scalar(20, 190, 255), bw);
           erode (bw, bw, cv::Mat(), cv::Point(-1,-1), 2);
           dilate (bw, bw, cv::Mat(), cv::Point(-1,-1), 1);
            blur(bw,bw,Size(3,3));//cvSmooth (bw,bw, CV_MEDIAN, 3, 1);
            imshow("Threshold", bw);



            Mat canny_output;
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;

            Canny( bw, canny_output, 50, 50*2, 3 );
            findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
            //dint s = findBiggestContour(contours);

            imshow("Canny", canny_output);

            Mat drawing = Mat::zeros( frame.size(), CV_8UC1 );
            for( int i = 0; i< contours.size(); i++ )
            {
               Scalar color = Scalar( rand()%255, rand()%255, rand()%255 );
               drawContours( frame, contours, i, color, 2, 8, hierarchy, 0, Point() );
            }

            //for ( int s = 0; s < contours.size(); s ++ )
              //drawContours( frame, contours, s, Scalar(rand() % 255,rand() % 255,rand() % 255), -1, 8, hierarchy, 0, Point() );

              //drawContours( frame, contours, s, Scalar(255), -1, 8, hierarchy, 0, Point() );



           imshow( "aaa", frame);
*/
       }
       else
       { printf(" --(!) No captured frame -- Break!"); break; }

       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
      }
   }
   return 0;
}
