#include <Servo.h>

Servo base, shoulder, elbow, claw;

int pos1=90;
int pos2=90;
int pos3=90;
int pos4=90;

void setup(){
  Serial.begin(9600);
  Serial.println("Let's Begin\n");
  base.attach(9);
  shoulder.attach(8);
  elbow.attach(6);
  claw.attach(7);
}


void loop(){
  base.write(90);
  shoulder.write(90);
  elbow.write(90);
  claw.write(90);
  delay(50);
//wait until a character is receieved
  while(1){
    if(Serial.available()){
      char ch = Serial.read();
//base control
      if(ch=='s'){
        if(pos4<180){
          pos4=pos4+5;
          Serial.println("base_left\n");
        }
      }
      if(ch=='S'){
        if(pos4>0){
          pos4=pos4-5;
          Serial.println("base_right\n");
        }
      }
      base.write(pos4);
      delay(50);
//shoulder control
      if(ch=='a'){
        if(pos3<180){
          pos3=pos3+5;
          Serial.println("shoulder_down\n");
        }
      }
      if(ch=='A'){
        if(pos3>0){
          pos3=pos3-5;
          Serial.println("shoulder_up\n");
        }
      }
      shoulder.write(pos3);
      delay(50);
//elbow control
      if(ch=='d'){
        if(pos2<180){
          pos2=pos2+5;
          Serial.println("elbow_up\n");
        }
      }
      if(ch=='D'){
        if(pos2>0){
          pos2=pos2-5;
          Serial.println("elbow_down\n");
        }
      }
      elbow.write(pos2);
      delay(50);
//claw control
      if(ch=='w'){
        if(pos1<180){
          //pos1=pos1+5;
          pos1 = 120;
          Serial.println("claw_close\n");
        }
      }
      if(ch=='W'){
        if(pos1>0){
          //pos1=pos1-5;
          pos1 = 90;
          Serial.println("claw_open\n");
        }
      }
      claw.write(pos1);
      delay(50);
//default positions
      if(ch=='q'){
        pos1=90;
        pos2=90;
        pos3=90;
        pos4=90;
        Serial.println("default positions\n");
        claw.write(pos1);
        base.write(pos2);
        elbow.write(pos3);
        shoulder.write(pos4);
        delay(50);
      }
    }
  }
}
