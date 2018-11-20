/*
Projet: PIRUS S1
Equipe: P-19
Auteurs: -Jeremie Bourque
         -Émile Dufour
Description: Librairie de toutes les variables globales.
Date: 15-11-2018
*/

#ifndef GLOBALS_H
#define GLOBALS_H

// pins pour controller le stepper motor.
const int pinDirection = 39;
const int pinStep = 37;

// hauteur des etages en pouce (ref. absolue à partir du sol).
const int nbEtages = 3;
const float etages [nbEtages] = {0, 12, 4}; // hauteur en pouce. 7.25
float hauteurActuelle = 0; // num d'étage, sol = 0

// constantes pour le moteur.
const int stepsParRev = 200;
const int pauseMin = 800; // min 600 a 12 V, 

// nb de filets par pouce de la vis
const int filetsParPouce = 12;

#endif
