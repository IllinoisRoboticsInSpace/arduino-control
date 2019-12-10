#include "SabertoothMotor.h"

SabertoothMotor s = SabertoothMotor ();
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  int a [] = {130,0,0,0,0};
  s.init(a);

}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available()>0) {
    String cmd = Serial.readString();
    if ((cmd == "begin\n") or (cmd == "begin")) {
      Serial.print("done");
    } else {
      s.read(cmd);
    }
  } 

}
