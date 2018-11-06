/*
Projet: PIRUS S1
Equipe: P-19
Auteurs: -Jeremie Bourque
         -Émile Dufour
Description: Librairie de toutes les variables globales.
Date: 06-11-2018
*/

#ifndef GLOBALS_H
#define GLOBALS_H

// pins pour controller le stepper motor.
const int pinDirection = 39;
const int pinStep = 37;

// hauteur des etages en pouce (ref. absolue à partir du sol).
const int nbEtages = 3;
const int etages [nbEtages] = {0, 1, 2}; // hauteur en pouce.
int etageActuel = 0; // num d'étage, sol = 0

// constantes pour le moteur.
const int stepsParRev = 200;
const int pauseMin = 700; // min 600
const int pauseMinManuel = 700; // min 600

// nb de filets par pouce de la vis
const int filetsParPouce = 12;

#endif
