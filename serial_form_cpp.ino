/*
#include <Servo.h>

Servo base, shoulder, elbow, claw;

//Servo positions
int pos1=90;
int pos2=90;
int pos3=90;
int pos4=90;

//incoming serial data
int incomingByte = 0;

void setup(){
  Serial.begin(9600);
  delay(50);
  Serial.println("Let's Begin\n");
  base.attach(9);
  shoulder.attach(8);
  elbow.attach(6);
  claw.attach(7);
  //set to defuault positions upon start
  base.write(90);
  shoulder.write(90);
  elbow.write(90);
  claw.write(90);
  delay(50);
}

void loop(){
  //read the incoming byte:
  incomingByte = Serial.read();
  switch(incomingByte){
    //case neutral position
    case '1':
      pos1+=1;
      delay(20);
      if(pos1>180){
        pos1 = 180;
      }
      break;
    //base control right
    case '2':
      pos1-=1;
      delay(20);
      if(pos1 < 0){
        pos1 = 0;
      }
      break;
  }
  base.write(pos1);
}

*/
#include <Servo.h>

Servo base;

int input,pos;
int buff[5];
//array buff of {val1, val2, val3, val4, va5}
//why 5?
int j=-1;

void setup()
{
    Serial.begin(9600);
    base.attach(9);
}


//since arduino receives the info as characters this is to change the characters back into numbers
int calc()
{
    int pos=0,x=0;
 
    for(x;x<=j;x++)
          pos=pos+(buff[x]-48)*pow(10,j-x);
          //pow(base, exponent)
 
    return pos;
}

 
void loop()
{
    if(Serial.available()>0)
    {
        input=Serial.read();
        
        if(input==',')
        {
          
            pos=calc();
            j=-1;
            Serial.println(pos);
            base.write(pos);
            delay(100);
            
        }
        else
        {
            j++;
            buff[j]=input;
            base.write(pos);
            delay(100);
        }
        
    }
}
