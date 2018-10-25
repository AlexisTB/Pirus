#ifndef GLOBALS_H
#define GLOBALS_H


// pins pour les boutons.
const int boutonUp = 36;
const int boutonDown = 42;
const int boutonCalibration = 50;

// pins pour controller le stepper motor.
const int pinDirection = 26;
const int pinStep = 27;

// hauteur des etages en pouce (ref. absolue à partir du sol).
const int nbEtages = 3;
const int etages [nbEtages] = {0, 3, 6}; // hauteur en pouce.
int etageActuel = 0; // num d'étage, sol = 0

// constantes pour le moteur.
const int stepsParRev = 200;
const int pauseMin = 600;
const int pauseMinManuel = 600;

// nb de filets par pouce de la vis
const int filetsParPouce = 12;

#endif
