#include "SabertoothMotor.h"

SabertoothMotor s = SabertoothMotor ();
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  int a [] = {130,0,0,0,0};
  s.init(a);

}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available()>0) {
    String cmd = Serial.readString();
    Serial.println(cmd); 
    s.read(cmd);
  } 

}
