//TODO SEE IF YOU CAN SEND INFO SIMILAR TO THAT OF THE BLOB DETECTION PROGRAM OVER TO SERIAL AND HAVE THE ARM MOVE TO A DIRECTION LIKE THAT

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

/*
int main()
{
  int data[] = {10,5,13};  //Random data we want to send
  FILE *file;
  file = fopen("/dev/ttyACM0","w");  //Opening device file
  if(file == NULL){
    cout<<"LOL YOU SUCK"<<endl;
  }
  int i = 0;
  for(i = 0 ; i < 3 ; i++)
    {
      fprintf(file,"%d",data[i]); //Writing to the file
      fprintf(file,"%c",','); //To separate digits
      sleep(1);
    }
  fclose(file);
}
*/

int posX;
int posY;

int main(){
  while(1){
    srand(time(NULL));

    posX = rand() % 180;
    posY = rand() % 180;

    int coordinates[] = {posX, posY};
    FILE *arduino;
    arduino = fopen("/dev/ttyACM0", "w");
    if(arduino == NULL){
      cout<<"error: cannot connect to /dev/ttyACM0"<<endl;
    }
    int i = 0;
    for(i =0; i<2; i++){
      cout<<"sending info"<<endl;
      fprintf(arduino, "%d", coordinates[i]);
      fprintf(arduino, "%c", ',');
      sleep(1);
    }
    cout<<"Info sent, I think"<<endl;
    cout<<posX<<", "<<posY<<endl;
    fclose(arduino);
  }
}
