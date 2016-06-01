//TODO INTEGRATE SENDING INFO OVER SERIAL TO ARDUINO
//TODO PERHAPS CHANGE THE INT MAIN INTO ITS OWN FUNCTION AND HAVE IT RETURN OR MODIFY THE iLastX AND iLastY VALUES TO BE SENT BY A SERIAL SENDING PROGRAM
//TODO FOR THE SERIAL INFO SEE IF YOU CAN USE ONE OF THE NUMBERS SENT TO CONTORL WHICH SERVO IS GOING TO BE USED

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
#include <math.h>

using namespace cv;
using namespace std;

//Position to send out to arduino to move servos
int posBase = 0;

//Distance from Red LED to Green Ball Top, and Side
int DistanceTop = 0;
int DistanceSide = 0;

//Variables for Red Blob Detection Trackbar
int LowH = 0;
int HighH = 179;

int LowS = 18;
int HighS = 255;

int LowV = 255;
int HighV = 255;

//Variables for Green Blob Detection Trackbar
int iLowH = 46;
int iHighH = 124;

int iLowS = 150; 
int iHighS = 255;

int iLowV = 60;
int iHighV = 255;

//Points for Green Object
int iLastX = -1;
int iLastY = -1;

//Points for Red Object
int LastX = -1;
int LastY = -1;

//Test when the points are closest
void pointsClosest(double){
  //sendInfo();
  //distanceFormula();
}

//Calculate the distance between the points
int distanceFormula(int, int, int, int){
  int result;

  DistanceTop = sqrt(pow((LastX-iLastX), 2) + pow((LastY-iLastY), 2));
  return DistanceTop;
  
}

void createTrackbarsRed(){
  namedWindow("Control Red", CV_WINDOW_AUTOSIZE);

  cvCreateTrackbar("LowH", "Control Red", &LowH, 179);
  cvCreateTrackbar("HighH", "Control Red", &HighH, 179);

  cvCreateTrackbar("LowS", "Control Red", &LowS, 255);
  cvCreateTrackbar("HighS", "Control Red", &HighS, 255);

  cvCreateTrackbar("LowV", "Control Red", &LowV, 255);
  cvCreateTrackbar("HighV", "Control Red", &HighV, 255);
}

void createTrackbarsGreen(){
  namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

  //Create trackbars in "Control" window
  cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
  cvCreateTrackbar("HighH", "Control", &iHighH, 179);

  cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
  cvCreateTrackbar("HighS", "Control", &iHighS, 255);

  cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
  cvCreateTrackbar("HighV", "Control", &iHighV, 255);
}


void sendInfo(){
  int information[]={posBase, posBase};
  FILE *arduino;
  arduino = fopen("/dev/ttyACM0", "w");
  if(arduino == NULL){
    cout<<"error: cannot connect to device /dev/ttyACM0"<<endl;
  }
  int i = 0;
  for(i = 0; i<2; i++){
    cout<<"sending info"<<endl;
    fprintf(arduino, "%d", information[i]);
    fprintf(arduino, "%c", ',');
    sleep(1);
  }
  cout<<"Information has been sent: "<<posBase<<endl;
  fclose(arduino);
}


int main( int argc, char** argv ){

    VideoCapture cap(1); //capture the video from webcam

    // if not success, exit program
    if ( !cap.isOpened() ){
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    createTrackbarsGreen();
    createTrackbarsRed();

    //Capture a temporary image from the camera
    Mat imgTmp;
    cap.read(imgTmp);

    //Create a black image with the size as the camera output
    Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;
    Mat imgLinesRed = Mat::zeros(imgTmp.size(), CV_8UC3);;


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
      Mat imgThresholdedRed;

      inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image for green

      inRange(imgHSV, Scalar(LowH, LowS, LowV), Scalar(HighH, HighS, HighV), imgThresholdedRed); //Threshold the image for red

      //morphological opening (removes small objects from the foreground)
      erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

      erode(imgThresholdedRed, imgThresholdedRed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      dilate(imgThresholdedRed, imgThresholdedRed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

      //morphological closing (removes small holes from the foreground)
      dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

      dilate( imgThresholdedRed, imgThresholdedRed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      erode(imgThresholdedRed, imgThresholdedRed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

      //Calculate the moments of the thresholded image
      Moments oMoments = moments(imgThresholded);
      Moments oMomentsred = moments(imgThresholdedRed);

      double dM01 = oMoments.m01;
      double dM10 = oMoments.m10;
      double dArea = oMoments.m00;

      double dM01Red = oMomentsred.m01;
      double dM10Red = oMomentsred.m10;
      double dAreaRed = oMomentsred.m00;

      // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zeros
      if (dArea > 9000 || dAreaRed > 500){

        //calculate the position of the Red LED
        int posXred = dM10Red / dAreaRed;
        int posYred = dM01Red / dAreaRed;

        //calculate the position of the ball
        int posX = dM10 / dArea;
        int posY = dM01 / dArea;

        if ((iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0) && (LastX >= 0 && LastY >= 0 && posXred >= 0 && posYred >= 0)){
          //Draw a red line from the previous point to the current point
          line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);

          line(imgLinesRed, Point(posXred, posYred), Point(LastX, LastY), Scalar(255,0,0), 2);
        }

        iLastX = posX;
        iLastY = posY;

        LastX = posXred;
        LastY = posYred;
      }

      imshow("Thresholded Image", imgThresholded); //show the thresholded image
      imshow("Thresholded Image Red", imgThresholdedRed);

      imgOriginal = imgOriginal + imgLines + imgLinesRed;
      imshow("Original", imgOriginal); //show the original image
      cout<<"X green position = "<<iLastX<<" Y green position = "<<iLastY<<endl;
      cout<<"X red position = "<<LastX<<" Y red position ="<<LastY<<endl;

      distanceFormula(iLastX, iLastY, LastX, LastY);
      cout<<DistanceTop<<endl;

      if (waitKey(30) == 27){ //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        cout << "esc key is pressed by user" << endl;
        break;
       }
    }

    return 0;
}
