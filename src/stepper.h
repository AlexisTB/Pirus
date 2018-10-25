#ifndef STEPPER_H
#define STEPPER_H
#include "globals.h"
#include <Arduino.h>

// Prototypes de fonction.
void elevation(float distance,  float vitesse);
void manuel(int sens);
void calibration();
void gotoEtage(int etage, float vitesse);

void setup() 
{
  Serial.begin(9600);
  pinMode(pinDirection,OUTPUT); // Step
  pinMode(pinStep,OUTPUT); // Dir
  pinMode(boutonUp, INPUT_PULLUP); // Up
  pinMode(boutonDown, INPUT_PULLUP); // Down
  pinMode(boutonCalibration, INPUT_PULLUP); // Calibration
}

void elevation(float distance,  float vitesse)
// distance (float) : distance d'elevation en pouce (+ = UP, - = DOWN).
// vitesse (float) : vitesse d'elevation (de 0 à 1).
{
  int steps = abs(stepsParRev * filetsParPouce * distance);
  int pause = pauseMin / vitesse;
  if(distance >= 0)
  {
    digitalWrite(pinDirection, HIGH); // set la direction pour monter.
    Serial.println("Sens = Up");
  }
  else
  {
    digitalWrite(pinDirection, LOW); // set la direction pour descendre.
    Serial.println("Sens = Down");
  }
  Serial.print("Vitesse = ");
  Serial.println(vitesse);
  Serial.print("Distance = ");
  Serial.println(distance);
  Serial.println(" ");
  for(int x = 0; x < steps; x++) // fait tourner le stepper motor le nb de steps desires.
  {
    digitalWrite(pinStep,HIGH); 
    delayMicroseconds(pause); 
    digitalWrite(pinStep,LOW);
    delayMicroseconds(pause);
  }
}

void manuel(int sens)
{
// sens (int) : 1 = monter, 0 = descendre.
  if(sens == 1)
  {
    digitalWrite(pinDirection, HIGH);
    Serial.println("manuel Up");
  }
  else
  {
    digitalWrite(pinDirection, LOW);
    Serial.println("manuel Down");
  }
  while(digitalRead(boutonUp) == LOW || digitalRead(boutonDown) == LOW)
  {
    digitalWrite(pinStep,HIGH); 
    delayMicroseconds(pauseMinManuel); 
    digitalWrite(pinStep,LOW);
    delayMicroseconds(pauseMinManuel);
  } 
}

void calibration()
// Calibration de l'origine.
{
  etageActuel = 0;
}

void gotoEtage(int etage, float vitesse)
// etage (int) : numero d'etage à laquelle ont veut aller
{
  Serial.print("Go to etage ");
  Serial.println(etage);
  float distance = etages[etage] - etages[etageActuel];
  elevation(distance, vitesse);
  etageActuel = etage;
  Serial.print("Etage actuel = ");
  Serial.println(etageActuel);
  Serial.println(" ");
}
#endif