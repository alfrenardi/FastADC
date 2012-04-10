#include "FastADC.h"

void setup(){
  FreeRunningADC.StartADC(A0,10); //pin A0, 10 bit
  Serial.begin(9600);
}

void loop(){
  int i = FreeRunningADC.Get();
  Serial.println(i, DEC);
}

