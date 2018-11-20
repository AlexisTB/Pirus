/*
Projet: PIRUS S1
Equipe: P-19
Auteurs: -Jeremie Bourque
         -Émile Dufour
Description: Robot de magasinage
Date: 15-11-2018
*/

/* ****************************************************************************
Inclure les librairies de functions que vous voulez utiliser
**************************************************************************** */

#include <Arduino.h>
#include "stepper.h"
#include "globals.h"
#include <LibRobus.h>

/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */

void setup() 
{
  // Pour le systeme d'elevation.
  Serial.begin(9600);
  pinMode(pinDirection,OUTPUT); // Step
  pinMode(pinStep,OUTPUT); // Dir
}

/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */

void loop() 
{
  calibration();
  delay(1000);
  while(1)
  {
    //Test
    gotoEtage(1,1);
    delay(1000);
    gotoEtage(0,1);
    delay(1000);
  }
}