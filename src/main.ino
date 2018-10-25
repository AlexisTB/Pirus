#include "STEPPER.h"
#include <Arduino.h>

void loop() 
{
  gotoEtage(2, 1);
  delay(1000);
  gotoEtage(0, 1);
  delay(1000);
  gotoEtage(1, 1);
  delay(1000);

  if(digitalRead(boutonUp) == LOW)
  {
    manuel(1);
  }
  else if(digitalRead(boutonDown) == LOW)
  {
    manuel(0);
  }
  else if(digitalRead(boutonCalibration) == LOW)
  {
    calibration();
  }
}