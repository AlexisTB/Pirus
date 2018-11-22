#ifndef SUIVRELIGNE_H
#define SUIVRELIGNE_H

#include <QTRSensors.h>
#include <LibRobus.h>
#include "globals.h"

void fetchMeSomeFoodBridget(int p_numeroAllee);
void calibrationAutoQuartDeTour(int p_sensAntiHoraire = 1);
void calibrationAutoPetitDeplacement(int p_deplaceVersLavant = 1, float distanceAParcourir = 1);
void calibrerSuiveurDeLigneAuto();
void calibrerSuiveurDeLigneManuel();
bool estUneLignePerpendiculaire();
int detectionLignePerpendiculaire();
void ligneDroiteVSTD(int p_nbLignesPerpendiculairesCible);
void ligneDroite(double p_vitesseCible, int p_nbLignesPerpendiculairesCible);

const float V_STD = 0.25 ; 
const int NB_ALLEES_EPICERIE = 3 ; 
const int GAUCHE = 0 ; 
const int DROITE = 1 ;
const double CENTRE = 2500 ; // valeur retournée par les capteurs lorsque la ligne est au centre
const float TEMPS_ATTENTE = 100.0;

//Pour les mouvements basés sur les encodeurs lors de la calibration 
//-------------------------------------------------
const int TICK_PAR_SECONDE_V_MAX = 10000;
const float TICK_PAR_TOUR = 3200.0;
const double CIRCONFERENCE = 7.7*PI;
const float DISTANCE_ENTRE_LES_ROUES = 18.3;
// ------------------------------------------------

//Pour déclarer l'objet capteur et les arrays qui y sont liés 
//-------------------------------------------------
const int NB_SENSORS = 8 ;
#define NUM_SAMPLES_PER_SENSOR  4  // Moyenne de 4 echantillons analogiques par lecture de sensor 
#define EMITTER_PIN QTR_NO_EMITTER_PIN  // fuck emettre
unsigned char pinsCapteurs[NB_SENSORS] {0,1,2,3,4,5,6,7};
unsigned int valeurSensors[NB_SENSORS] {0,0,0,0,0,0,0,0};
QTRSensorsAnalog capteurs(pinsCapteurs , NB_SENSORS,NUM_SAMPLES_PER_SENSOR,EMITTER_PIN);
// ------------------------------------------------
// void fetchMeSomeFoodBridget(int p_numeroAllee)
// {

// }
void ReculerDistance(float distanceAParcourir){
  MOTOR_SetSpeed(GAUCHE, -0.2);
  MOTOR_SetSpeed(DROITE,  -0.2);
  float distanceCumuleGauche = 0 ;
  float distanceCumuleDroite = 0 ;
  ENCODER_ReadReset(GAUCHE);
  ENCODER_ReadReset(DROITE);
  while (distanceCumuleGauche < distanceAParcourir || distanceCumuleDroite < distanceAParcourir)
  {
    distanceCumuleGauche = -CIRCONFERENCE * ENCODER_Read(GAUCHE)/TICK_PAR_TOUR;
    distanceCumuleDroite = -CIRCONFERENCE * ENCODER_Read(DROITE)/TICK_PAR_TOUR;

    if (distanceCumuleGauche > distanceAParcourir)
    {
      MOTOR_SetSpeed(GAUCHE, 0);
    }
    if (distanceCumuleDroite > distanceAParcourir)
    {
      MOTOR_SetSpeed(DROITE, 0);
    }
  }
  delay(250);
}

void AvancerTemps(int temps){
  MOTOR_SetSpeed(GAUCHE,.2);
  MOTOR_SetSpeed(DROITE,.2);
  delay(temps);
  MOTOR_SetSpeed(GAUCHE, 0);
  MOTOR_SetSpeed(DROITE, 0);
}

void dropItem(){
  MOTOR_SetSpeed(GAUCHE,.4);
  MOTOR_SetSpeed(DROITE,.4);
  delay(100);
  MOTOR_SetSpeed(GAUCHE,1);
  MOTOR_SetSpeed(DROITE,1);
  delay(200);
  MOTOR_SetSpeed(GAUCHE, 0);
  MOTOR_SetSpeed(DROITE, 0);
  delay(10);
  MOTOR_SetSpeed(GAUCHE,-.4);
  MOTOR_SetSpeed(DROITE,-.4);
  delay(100);
  MOTOR_SetSpeed(GAUCHE,-1);
  MOTOR_SetSpeed(DROITE,-1);
  delay(200);
  MOTOR_SetSpeed(GAUCHE, 0);
  MOTOR_SetSpeed(DROITE, 0);
  delay(200);
}

void calibrerSuiveurDeLigneAuto()
// si tu feel lâche aujourd'hui (#BitchImNotLazyImFabulous) : 
// 1) Place le à peu près au centre de la ligne 2) be amazed
{
    calibrationAutoPetitDeplacement(1,CIRCONFERENCE/4);
    calibrationAutoQuartDeTour(); // Un quart de tour anti horaire 
    calibrationAutoPetitDeplacement(-1,CIRCONFERENCE/2);
    calibrationAutoPetitDeplacement(1,CIRCONFERENCE/2);
    calibrationAutoQuartDeTour(-1); // Un quart de tour horaire 
}

void calibrerSuiveurDeLigneManuel()
// si ça te tente de te faire chier à faire glisser le robot toi même sur la ligne 
{
    for (int i = 0; i < 50; i++)  // calibration d'à peu près 1.5 secondes
    {
        capteurs.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
    }
}

bool estUneLignePerpendiculaire()
{
    const int NB_TESTS = 10 ; 
    int nbTestsPositifs = 0 ; 
    for (int i = 0 ; i < NB_TESTS; i++)
    {
        delay(TEMPS_ATTENTE/NB_TESTS);
        nbTestsPositifs += detectionLignePerpendiculaire();
    }
    return nbTestsPositifs >= (3) ; 
}

bool detectionLignePerpendiculaireAlternative()
{
    int nbPositifs= 0 ;
    int i = 0 ;
    capteurs.readCalibrated(valeurSensors);
    while (i < NB_SENSORS)
    {
        if (valeurSensors[i] > 500)
        {
            nbPositifs ++ ; 
        };
        i ++ ; 
    }
    return nbPositifs > (NB_SENSORS -3); 
}

int detectionLignePerpendiculaire()
{
    int estUneLignePerpendiculaire = 1 ;
    int i = 0 ;
    capteurs.readCalibrated(valeurSensors);
    while (i < NB_SENSORS && estUneLignePerpendiculaire)
    {
        if (valeurSensors[i] < 350)
        {
            estUneLignePerpendiculaire = 0 ; 
        };
        i ++ ; 
    }
    SOFT_TIMER_Update();
    return estUneLignePerpendiculaire; 
}

void ligneDroiteVSTD(int p_nbLignesPerpendiculairesCible)
{
  ligneDroite(V_STD,p_nbLignesPerpendiculairesCible) ; 
}

void ligneDroite(double p_vitesseCible, int p_nbLignesPerpendiculairesCible)
{
  const double P = 0.0000200*1.25;
  const double I = 0.0000004;
  const double D = 0.0000500;
  double erreurI = 0 ; 
  double ancienneErreur = 0 ; 
  int nbLignesPerpendiculairesRencontrees = 0 ; 
  int nbPositifConsecutifsLignePerp = 0 ; 

  MOTOR_SetSpeed(GAUCHE, p_vitesseCible);
  MOTOR_SetSpeed(DROITE, p_vitesseCible);

  while(nbLignesPerpendiculairesRencontrees < p_nbLignesPerpendiculairesCible)
  {
    int i = 0 ; 
    while (i < 10 && nbPositifConsecutifsLignePerp < 5)
    {
      if (detectionLignePerpendiculaireAlternative())
      {
        nbPositifConsecutifsLignePerp ++ ;
      }
      else 
      {
        nbPositifConsecutifsLignePerp = 0 ; 
      }
      
      i ++;
      delay(TEMPS_ATTENTE/10);
    }
    if (nbPositifConsecutifsLignePerp >= 5)
    {
      nbLignesPerpendiculairesRencontrees ++;
      nbPositifConsecutifsLignePerp = 0 ; 
      AX_BuzzerON(2000,100);
    }
    else if (!detectionLignePerpendiculaire())
    {
      int position = capteurs.readLine(valeurSensors);

      double erreurP = position - CENTRE;
      erreurI += erreurP  ; 
      double erreurD = erreurP - ancienneErreur;
      ancienneErreur = erreurP;

      MOTOR_SetSpeed(DROITE, p_vitesseCible + P * erreurP + I * erreurI + D * erreurD);
    }

  }
  AvancerTemps(300);
 }

void calibrationAutoPetitDeplacement(int p_deplaceVersLavant, float p_distanceAParcourir)
// p_deplaceVersLavant = 1 --> avance
// p_deplaceVersLavant = -1 --> recule

{
  MOTOR_SetSpeed(GAUCHE, p_deplaceVersLavant  * 0.2);
  MOTOR_SetSpeed(DROITE,  p_deplaceVersLavant * 0.2);
  float distanceCumuleGauche = 0 ;
  ENCODER_ReadReset(GAUCHE);
  while (distanceCumuleGauche < p_distanceAParcourir)
  {
    distanceCumuleGauche = p_deplaceVersLavant * CIRCONFERENCE * ENCODER_Read(GAUCHE)/TICK_PAR_TOUR;
    capteurs.calibrate(); //(~25 ms per call)
  }
  MOTOR_SetSpeed(GAUCHE, 0);
  MOTOR_SetSpeed(DROITE, 0);
}

void calibrationAutoQuartDeTour(int p_sensAntiHoraire)
// p_sensAntiHoraire = 1 --> tourne en sens anti horaire 
// p_sensAntiHoraire = -1 --> tourne en sens horaire 
{
  float distanceAParcourir = (DISTANCE_ENTRE_LES_ROUES*2*PI)/8.0;
  MOTOR_SetSpeed(GAUCHE, p_sensAntiHoraire  * 0.2);
  MOTOR_SetSpeed(DROITE,  p_sensAntiHoraire * -0.2);
  float distanceCumuleGauche = 0 ;
  float distanceCumuleDroite = 0 ;
  ENCODER_ReadReset(GAUCHE);
  ENCODER_ReadReset(DROITE);
  while (distanceCumuleGauche < distanceAParcourir || distanceCumuleDroite < distanceAParcourir)
  {
    distanceCumuleGauche = p_sensAntiHoraire * CIRCONFERENCE * ENCODER_Read(GAUCHE)/TICK_PAR_TOUR;
    distanceCumuleDroite = p_sensAntiHoraire * -CIRCONFERENCE * ENCODER_Read(DROITE)/TICK_PAR_TOUR;

    if (distanceCumuleGauche > distanceAParcourir)
    {
      MOTOR_SetSpeed(GAUCHE, 0);
    }
    if (distanceCumuleDroite > distanceAParcourir)
    {
      MOTOR_SetSpeed(DROITE, 0);
    }
    capteurs.calibrate(); //(~25 ms per call)
  }
}
void demiTour()
{
  float distanceAParcourir = (DISTANCE_ENTRE_LES_ROUES*2*PI)/3.87; //3.80
  MOTOR_SetSpeed(GAUCHE, 0.2);
  MOTOR_SetSpeed(DROITE,  -0.2);
  float distanceCumuleGauche = 0 ;
  float distanceCumuleDroite = 0 ;
  ENCODER_ReadReset(GAUCHE);
  ENCODER_ReadReset(DROITE);
  while (distanceCumuleGauche < distanceAParcourir || distanceCumuleDroite < distanceAParcourir)
  {
    distanceCumuleGauche = CIRCONFERENCE * ENCODER_Read(GAUCHE)/TICK_PAR_TOUR;
    distanceCumuleDroite = -CIRCONFERENCE * ENCODER_Read(DROITE)/TICK_PAR_TOUR;

    if (distanceCumuleGauche > distanceAParcourir)
    {
      MOTOR_SetSpeed(GAUCHE, 0);
    }
    if (distanceCumuleDroite > distanceAParcourir)
    {
      MOTOR_SetSpeed(DROITE, 0);
    }
  }
  delay(750);
}
void testThisShit()
{
  ligneDroiteVSTD(3); 
  delay(1750);
  MOTOR_SetSpeed(GAUCHE,0);
  MOTOR_SetSpeed(DROITE,0);
  delay(100);
  demiTour();
}
void lireCapteurs()
{
  capteurs.readCalibrated(valeurSensors);
  for (int i = 0 ; i < 8 ; i++)
  {
    Serial.print(valeurSensors[i]);
    Serial.print(" ");
  }
  Serial.println();
}

#endif