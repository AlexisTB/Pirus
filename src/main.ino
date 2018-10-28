/*
Projet: PIRUS S1
Equipe: P-19
Auteurs: -Jeremie Bourque
         -
Description: Robot de magasinage
Date: 28-10-2018
*/

/* ****************************************************************************
Inclure les librairies de functions que vous voulez utiliser
**************************************************************************** */

#include <Arduino.h>
#include "stepper.h"
#include "globals.h"

/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */

void setup() 
{
  // Pour le systeme d'elevation.
  Serial.begin(9600);
  pinMode(pinDirection,OUTPUT); // Step
  pinMode(pinStep,OUTPUT); // Dir
  pinMode(boutonUp, INPUT_PULLUP); // Up
  pinMode(boutonDown, INPUT_PULLUP); // Down
  pinMode(boutonCalibration, INPUT_PULLUP); // Calibration
}

/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */

void loop() 
{
  // Tests systeme d'elevation.
  delay(1000);
  gotoEtage(1, 1);
  delay(1000);
  gotoEtage(0, 1);

  // Main.
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