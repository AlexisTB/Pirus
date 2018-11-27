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
#include "melody.h"
#include <LibRobus.h>

#define TIMER 3

String messageRecu;
int state = 0;

/* ****************************************************************************
Fonctions
**************************************************************************** */

void RecevoirMessage(){
  AX_BuzzerON(2500,1500);
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
  AX_BuzzerON(2000,100);
  calibration();
  AX_BuzzerON(1000,100);

   //Bluetooth
  BluetoothInit();
  delay(1000);
  BLUETOOTH_setCallback(RecevoirMessage);

  delay(500);
  AX_BuzzerON(1000,100);
 // Hardcode d'une commande de l'aliment 0.
   //state = 1;
   //messageRecu = "2";
}

/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
void loop() 
{ 
  // Please don't fuck up Bridget Bridgestone 
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

      ligneDroiteVSTD(x+2); // se rendre à l'allee
      calibrationAutoQuartDeTour(1); // Un quart de tour horaire 
      
      gotoEtage(y,1);
      ligneDroiteVSTD(1,1,0); // se rendre au bout de l'allée 
      elevation(0.9,1);
      ReculerDistance(15);
      demiTour();
      ligneDroiteVSTD(1); // retourner à l'allee
      delay(250);
      calibrationAutoQuartDeTour(-1); // Un quart de tour antihoraire
      if(hauteurActuelle < etages[1])
      {
        gotoEtage(1,1);
      } 
      ligneDroiteVSTD(x+1,0,900); // se rendre à la ligne de départ
      dropItem();
      demiTour();

    }

    state = 0; //mode attente
   // Good job Bridget ! 
   AX_BuzzerON(880,200);
   delay(200);
   AX_BuzzerON(932.33,200);
   delay(200);
   AX_BuzzerON(987.77,200);
   delay(200);
   AX_BuzzerON(1046.5,1000);
  }
	
  

}
