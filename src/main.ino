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
#include "suivreligne.h"
#include <LibRobus.h>

#define TIMER 3

String messageRecu;
int state = 0;

/* ****************************************************************************
Fonctions
**************************************************************************** */

void RecevoirMessage(){
  AX_BuzzerON(5000,1000);
  Serial.print("YO j'ai recu un MESSAGE: ");
  messageRecu = BLUETOOTH_read();
  Serial.println(messageRecu);
  state = 1;
}

void FonctionTestTempsGlobal(){

  // Serial.print("temps = ");
  // Serial.println(tempsGlobal);
  tempsGlobal++;

}

/* ****************************************************************************
Fonctions d'initialisation
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois

void setup(){

  BoardInit();

  //pour le temps 
  SOFT_TIMER_SetCallback(TIMER,&FonctionTestTempsGlobal);
  SOFT_TIMER_SetDelay(TIMER,1000);
  SOFT_TIMER_SetRepetition(TIMER,-1);
  SOFT_TIMER_Enable(TIMER);

  // Pour le systeme d'elevation.
  pinMode(pinDirection,OUTPUT); // Step
  pinMode(pinStep,OUTPUT); // Dir

  calibrerSuiveurDeLigneAuto();
  calibration();

   //Bluetooth
  BluetoothInit();
  delay(1000);
  BLUETOOTH_setCallback(RecevoirMessage);

  delay(500);
  AX_BuzzerON(1000,100);
}

/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */

void loop() 
{ 
  SOFT_TIMER_Update();

  if (state == 1) {
    //si message est reçu (mode recherche)
    String str = messageRecu;
    int longueur = str.length();
    int liste[longueur];

    char caractere;
    int chiffre;
    
    for (int i = 0; i < longueur; i++)
    { //converti [] char en [] int
    
    caractere = str[i];
    chiffre = caractere - '0';

    liste[i] = chiffre;
    
    }
    //        
    //    | 8 7 6 |
    // y  | 5 4 3 |
    //    | 2 1 0 |
    //        x
    
    for(int i = 0; i < longueur; i++)
    {
      
      int y = liste[i] / 3;
      int x = liste[i] -  3* y;

      Serial.print("x = ");
      Serial.println(x);
      Serial.print("y = ");
      Serial.println(y);

      //calibrationAutoPetitDeplacement(1,CIRCONFERENCE/2); // pour pas lire la ligne de départ comme une première ligne perpendiculaire
      ligneDroiteVSTD(x+1); // se rendre à l'allee
      calibrationAutoQuartDeTour(1); // Un quart de tour horaire 
      
      gotoEtage(y,1);
      ligneDroiteVSTD(1); // se rendre au bout de l'allée 
      elevation(0.5,1);

      ReculerTemps(1500);
      demiTour();
      ligneDroiteVSTD(1); // retourner à l'allee
      calibrationAutoQuartDeTour(-1); // Un quart de tour antihoraire 
      ligneDroiteVSTD(x+1); // se rendre à la ligne de départ
      //dropItem();
      demiTour();
      //AvancerTemps(500);

    }

    state = 0; //mode attente
  }
	
  

}