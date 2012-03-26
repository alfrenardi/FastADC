#include "FastADC.h"

void setup(){
  FreeRunningADC.StartADCRes(0);
  Serial.begin(9600);
}

void loop(){
  int i = FreeRunningADC.Get();
  Serial.println(i, DEC);
}

