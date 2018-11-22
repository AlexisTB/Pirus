/*
Projet: PIRUS S1
Equipe: P-19
Auteurs: -Jeremie Bourque
         -Émile Dufour
Description: Librairie des fonctions du stepper motor pour le systeme d'elevation.
Date: 15-11-2018
*/

/*TODO
  Monter un peu apres avoir pris la charge 
  Ajouter Or bumper switch pour la descente
  Le prog doit connaitre sa hauteur en po, pas son etage
*/

#ifndef STEPPER_H
#define STEPPER_H
#include "globals.h"
#include <Arduino.h>
#include <LibRobus.h>

// Prototypes de fonction.
void elevation(float distance,  float vitesse);
void gotoEtage(int etage, float vitesse);
void calibration();


void elevation(float distance,  float vitesse)
// distance (float) : distance d'elevation en pouce (+ = UP, - = DOWN).
// vitesse (float) : vitesse d'elevation (de 0 à 1).
{
  int steps = abs(stepsParRev * filetsParPouce * distance);
  // Serial.print("Nb de steps = ");
  // Serial.println(steps);
  int pause = pauseMin / vitesse;
  if(distance >= 0)
  {
    digitalWrite(pinDirection, HIGH); // set la direction pour monter.
    // Serial.println("Sens = Up");
  }
  else
  {
    digitalWrite(pinDirection, LOW); // set la direction pour descendre.
    // Serial.println("Sens = Down");
  }
  // Serial.print("Vitesse = ");
  // Serial.println(vitesse);
  // Serial.print("Distance = ");
  // Serial.println(distance);
  for(int x = 0; x < steps; x++) // fait tourner le stepper motor le nb de steps desires.
  {
    if(ROBUS_IsBumper(2) && distance < 0)
    {
      // Serial.println("break");
      break;
    }
    digitalWrite(pinStep,HIGH); 
    delayMicroseconds(pause); 
    digitalWrite(pinStep,LOW);
    delayMicroseconds(pause);
  }
  
  hauteurActuelle = hauteurActuelle + distance;
  // Serial.print("Hauteur actuelle = ");
  // Serial.println(hauteurActuelle);
  // Serial.println("");
}

void calibration()
// Calibration de l'origine.
{
  // Serial.println("Calibration");
  digitalWrite(pinDirection, LOW); // set la direction pour descendre
  while(!ROBUS_IsBumper(2))
  {
    digitalWrite(pinStep,HIGH); 
    delayMicroseconds(pauseMin); 
    digitalWrite(pinStep,LOW);
    delayMicroseconds(pauseMin);
  }
  hauteurActuelle = 0;
  
}

void gotoEtage(int etage, float vitesse)
// etage (int) : numero d'etage à laquelle ont veut aller
{
  // Serial.print("Go to etage ");
  // Serial.println(etage);
  float distance = etages[etage] - hauteurActuelle;
  // unsigned long startTime = millis(); // Depart du chrono
  elevation(distance, vitesse);
  // unsigned long endTime = millis(); // Fin du chrono
  // double elapsedTime = (endTime - startTime)/1000.0; // Calcul du temps pour l'elevation
  // Serial.print("Temps = ");
  // Serial.println(elapsedTime);
  // Serial.print("Vitesse (sec/po) = ");
  // Serial.println(abs(float(elapsedTime)/distance)); //Calcul de la vitesse lineaire (sec/po)
  // Serial.print("RPM = ");
  // Serial.println(abs(distance*filetsParPouce/elapsedTime*60)); //Calcul du RPM
  // Serial.println(" ");
  
}
#endif