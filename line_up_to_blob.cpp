
//TODO PRINT OUT THE RESULTS OF X AND Y, NO NEED FOR THE FANCY DRAWING THE PLACES OF THE BALL AS LONG AS YOU HAVE THE INFORMATION OF WHERE IT IS
//TODO FIGURE OUT HOW TO TRACK TWO DIFFERENT OBJECTS AND RECORD THEIR OWN X AND Y POSITIONS

#include<iostream>
#include<sstream>
#include<string>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<stdio.h>

using namespace cv;
using namespace std;

int iLowH = 0;
int iHighH = 179;

int iLowS = 0; 
int iHighS = 255;

int iLowV = 0;
int iHighV = 255;


int iLowH1 = 0;
int iHighH1 = 179;

int iLowS1 = 0; 
int iHighS1 = 255;

int iLowV1 = 0;
int iHighV1 = 255;

int main(){
  VideoCapture cap(2); //capture the video from webcam
  VideoCapture cap1(1);

  if ( !cap.isOpened() && !cap1.isOpened())  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

 int iLowH = 170;
 int iHighH = 179;

 int iLowS = 150; 
 int iHighS = 255;

 int iLowV = 60;
 int iHighV = 255;

 int iLowH1 = 170;
 int iHighH1 = 179;

 int iLowS1 = 150; 
 int iHighS1 = 255;

 int iLowV1 = 60;
 int iHighV1 = 255;

 //Create trackbars in "Control" window
 createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
 createTrackbar("HighH", "Control", &iHighH, 179);

 createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 createTrackbar("HighS", "Control", &iHighS, 255);

 createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
 createTrackbar("HighV", "Control", &iHighV, 255);


 createTrackbar("LowH1", "Control", &iLowH1, 179); //Hue (0 - 179)
 createTrackbar("HighH1", "Control", &iHighH1, 179);

 createTrackbar("LowS1", "Control", &iLowS1, 255); //Saturation (0 - 255)
 createTrackbar("HighS1", "Control", &iHighS1, 255);

 createTrackbar("LowV1", "Control", &iLowV1, 255);//Value (0 - 255)
 createTrackbar("HighV1", "Control", &iHighV1, 255);

 int iLastX = -1; 
 int iLastY = -1;

 int iLastX1 = -1;
 int iLastY1 = -1;

 //Capture a temporary image from the camera
 Mat imgTmp;
 cap.read(imgTmp);
 Mat imgTmp1;
 cap1.read(imgTmp1);

 //Create a black image with the size as the camera output
 Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;
 Mat imgLines1 = Mat::zeros( imgTmp1.size(), CV_8UC3);;

    while (true)
    {
        Mat imgOriginal;
        Mat imgOriginal1;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video
        bool bSuccess1 = cap1.read(imgOriginal1);

         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }

         Mat imgHSV;
         Mat imgHSV1;

         cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
         cvtColor(imgOriginal1, imgHSV1, COLOR_BGR2HSV);
 
  Mat imgThresholded;
  Mat imgThresholded1;

  inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

  inRange(imgHSV1, Scalar(iLowH1, iLowS1, iLowV1), Scalar(iHighH1, iHighS1, iHighV1), imgThresholded1); //Threshold the image

  //morphological opening (removes small objects from the foreground)
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

  erode(imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 


  //morphological closing (removes small holes from the foreground)
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

  dilate( imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
  erode(imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );


  //Calculate the moments of the thresholded image
  Moments oMoments = moments(imgThresholded);
  Moments oMoments1 = moments(imgThresholded1);

  double dM01 = oMoments.m01;
  double dM10 = oMoments.m10;
  double dArea = oMoments.m00;

  double dM011 = oMoments1.m01;
  double dM101 = oMoments1.m10;
  double dArea1 = oMoments1.m00;

  // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero
  if (dArea > 10000)
  {
   //calculate the position of the ball
   int posX = dM10 / dArea;
   int posY = dM01 / dArea;

   int posX1 = dM101 / dArea1;
   int posY1 = dM011 / dArea1;

   if ((iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0) && (iLastX1 >= 0 && iLastY1 >= 0 && posX1 >= 0 && posY1 >= 0) )
   {
    //Draw a red line from the previous point to the current point
    //line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
     cv::circle(imgLines, Point(iLastX, iLastY), 3, Scalar(0,0,255), 22);
     cv::circle(imgLines1, Point(iLastX1, iLastY1), 3, Scalar(0,0,255), 22);

     // cv::circle(imgLines, Point(posX, posY), 3, Scalar(0,0,0), CV_FILLED);
     // cv::circle(imgLines1, Point(posX1, posY1), 3, Scalar(0,0,0), CV_FILLED);

     cout<<"top x is"<< posX<< "top y is"<< posY<< "\n"<<endl;
     cout<<"side x is"<< posX1<< "side y is"<< posY1<< "\n"<<endl;

   }

   iLastX = posX;
   iLastY = posY;

   iLastX1 = posX1;
   iLastY1 = posY1;

   /*
  if (iLastX1 >= 0 && iLastY1 >= 0 && posX1 >= 0 && posY1 >= 0)
    {
      //Draw a red line from the previous point to the current point
      //line(imgLines, Point(posX1, posY1), Point(iLastX1, iLastY1), Scalar(0,0,255), 2);
      cv::circle(imgLines1, Point(iLastX1, iLastY1), 20, Scalar(0,0,255), 2);
    }

  iLastX1 = posX1;
  iLastY1 = posY1;
   */
  }
    imshow("Thresholded Image 1", imgThresholded1);
    imshow("Thresholded Image", imgThresholded); //show the thresholded image

    //imshow("Thresholded Image 1", imgThresholded1);

  imgOriginal = imgOriginal + imgLines;
  imshow("Original", imgOriginal); //show the original image

  imgOriginal1 = imgOriginal1 +imgLines1;
  imshow("Orginal 1", imgOriginal1);

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break;
       }
    }

   return 0;



}
