//TODO PERHAPS YOU NEED TWO TRACKBARS FOR EACH COLOR FOR EACH CAMERA, THUS YOU HAVE 8 WINDOWS TOTAL, 2 FOR GREEN, 2 FOR RED ALL FOR TOP CAMERA, 2 FOR GREEN, 2 FOR RED ALL FOR SIDE CAMERA
//TODO SEE IF YOU CAN SENSE RED AT THE SAME TIME, PROBABLY GOING TO NEED A SEPARATE WINDOW AND TRACKBAR OR SOMETHING
//TODO INTEGRATE SENDING INFO OVER SERIAL TO ARDUINO

#include <iostream>
#include <sstream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

using namespace cv;
using namespace std;

int iLowH = 46;
int iHighH = 124;

int iLowS = 150; 
int iHighS = 255;

int iLowV = 60;
int iHighV = 255;

void createTrackbars(){
  namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

  //Create trackbars in "Control" window
  cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
  cvCreateTrackbar("HighH", "Control", &iHighH, 179);

  cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
  cvCreateTrackbar("HighS", "Control", &iHighS, 255);

  cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
  cvCreateTrackbar("HighV", "Control", &iHighV, 255);
}

int main( int argc, char** argv ){

    VideoCapture cap(1); //capture the video from webcam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    createTrackbars();

    int iLastX = -1;
    int iLastY = -1;

    //Capture a temporary image from the camera
    Mat imgTmp;
    cap.read(imgTmp);

    //Create a black image with the size as the camera output
    Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;


    while (true){

      Mat imgOriginal;
      bool bSuccess = cap.read(imgOriginal); // read a new frame from video
      if (!bSuccess){ //if not success, break loop
        cout << "Cannot read a frame from video stream" << endl;
        break;
        }

      Mat imgHSV;

      cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

      Mat imgThresholded;

      inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

      //morphological opening (removes small objects from the foreground)
      erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

      //morphological closing (removes small holes from the foreground)
      dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

      //Calculate the moments of the thresholded image
      Moments oMoments = moments(imgThresholded);

      double dM01 = oMoments.m01;
      double dM10 = oMoments.m10;
      double dArea = oMoments.m00;

      // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zeros
      if (dArea > 10000){

        //calculate the position of the ball
        int posX = dM10 / dArea;
        int posY = dM01 / dArea;
        if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0){
          //Draw a red line from the previous point to the current point
          line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
        }

        iLastX = posX;
        iLastY = posY;
      }

      imshow("Thresholded Image", imgThresholded); //show the thresholded image

      imgOriginal = imgOriginal + imgLines;
      imshow("Original", imgOriginal); //show the original image
      cout<<"X position = "<<iLastX<<" Y position = "<<iLastY<<endl;

      if (waitKey(30) == 27){ //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        cout << "esc key is pressed by user" << endl;
        break;
       }
    }
    return 0;
}
