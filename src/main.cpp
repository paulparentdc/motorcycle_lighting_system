#include <Arduino.h>
#include "configuration.h"

//////////////////////////////////////////////////////
// Informations sur les sorties des modules TLC5940 //
//////////////////////////////////////////////////////

// Sortie n°0 = cinquième colonne de LEDs du clignotant avant gauche
// Sortie n°1 = quatrième colonne de LEDs du clignotant avant gauche
// Sortie n°2 = troisième colonne de LEDs du clignotant avant gauche
// Sortie n°3 = deuxième colonne de LEDs du clignotant avant gauche
// Sortie n°4 = première colonne de LEDs du clignotant avant gauche

// Sortie n°5 = première colonne de LEDs du clignotant avant droit
// Sortie n°6 = deuxième colonne de LEDs du clignotant avant droit
// Sortie n°7 = troisième colonne de LEDs du clignotant avant droit
// Sortie n°8 = quatrième colonne de LEDs du clignotant avant droit
// Sortie n°9 = cinquième colonne de LEDs du clignotant avant droit

// Sortie n°10 = cinquième colonne de LEDs du clignotant arrière droit
// Sortie n°11 = quatrième colonne de LEDs du clignotant arrière droit
// Sortie n°12 = troisième colonne de LEDs du clignotant arrière droit
// Sortie n°13 = deuxième colonne de LEDs du clignotant arrière droit
// Sortie n°14 = première colonne de LEDs du clignotant arrière droit

// Sortie n°15 = première colonne de LEDs du clignotant arrière gauche
// Sortie n°16 = deuxième colonne de LEDs du clignotant arrière gauche
// Sortie n°17 = troisième colonne de LEDs du clignotant arrière gauche
// Sortie n°18 = quatrième colonne de LEDs du clignotant arrière gauche
// Sortie n°19 = cinquième colonne de LEDs du clignotant arrière gauche

// Sortie n°20 = Couleur verte du bandeau de LEDs RGB qui éclaire la plaque d'immatriculation
// Sortie n°21 = Couleur bleue du bandeau de LEDs RGB qui éclaire la plaque d'immatriculation
// Sortie n°22 = Couleur rouge du bandeau de LEDs RGB qui éclaire la plaque d'immatriculation

// Sortie n°23 = Couleur verte du bandeau de LEDs RGB qui éclaire le phare avant
// Sortie n°24 = Couleur bleue du bandeau de LEDs RGB qui éclaire le phare avant
// Sortie n°25 = Couleur rouge du bandeau de LEDs RGB qui éclaire le phare avant

// Sortie n°26 = Couleur verte du bandeau de LEDs RGB qui éclaire le tableau de bord
// Sortie n°27 = Couleur bleue du bandeau de LEDs RGB qui éclaire le tableau de bord
// Sortie n°28 = Couleur rouge du bandeau de LEDs RGB qui éclaire le tableau de bord

// Sortie n°29 = BIP sonore de signalisation des clignotants et warnings
// Sortie n°30 = voyant vert de signalisation du fonctionnement des clignotants gauches
// Sortie n°31 = voyant vert de signalisation du fonctionnement des clignotants droits

/////////////////////////////////////////////////////////////
//        Commande par Smartphone via le bluetooth         //
/////////////////////////////////////////////////////////////

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__SOFTSERIAL
#include <SoftwareSerial.h>

#include <RemoteXY.h>

// RemoteXY connection settings
#define REMOTEXY_SERIAL_RX 0
#define REMOTEXY_SERIAL_TX 1
#define REMOTEXY_SERIAL_SPEED 9600

// RemoteXY configurate
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
    {255, 10, 0, 0, 0, 156, 0, 10, 27, 0,
     2, 0, 4, 3, 22, 11, 121, 26, 31, 31,
     79, 78, 0, 79, 70, 70, 0, 1, 0, 5,
     34, 12, 12, 120, 31, 60, 0, 1, 0, 19,
     34, 12, 12, 50, 31, 87, 0, 1, 0, 33,
     34, 12, 12, 120, 31, 62, 0, 1, 0, 5,
     50, 12, 12, 37, 31, 75, 0, 129, 0, 29,
     6, 18, 6, 30, 77, 111, 116, 101, 117, 114,
     0, 129, 0, 64, 10, 28, 11, 24, 49, 50,
     53, 32, 76, 0, 129, 0, 61, 3, 35, 6,
     24, 77, 111, 116, 111, 99, 111, 110, 102, 111,
     114, 116, 0, 2, 0, 22, 51, 22, 11, 190,
     26, 31, 31, 80, 70, 0, 67, 79, 68, 69,
     0, 3, 131, 5, 17, 40, 15, 2, 26, 4,
     0, 87, 19, 8, 42, 2, 26, 4, 0, 73,
     19, 8, 42, 2, 26, 4, 0, 59, 19, 8,
     42, 2, 26};

// this structure defines all the variables and events of your control interface
struct
{

  // variables d'entrées
  uint8_t moteur;     // =1 si l'interrupteur est sur ON et =0 s'il est sur OFF
  uint8_t turn_left;  // =1 si le bouton cligno gauche est appuyé, sinon =0
  uint8_t warn;       // =1 si le bouton warning est appuyé, sinon =0
  uint8_t turn_right; // =1 si le bouton cligno droit est appuyé, sinon =0
  uint8_t klaxon;     // =1 si le bouton klaxon est appuyé, sinon =0
  uint8_t light;      // =1 si l'interrupteur lumiere est sur ON et =0 s'il est sur OFF
  uint8_t OVR;        // =0 si la commande OVR est sur A, =1 si elle est sur B, =2 si elle est sur C, ...
  int8_t slider_1;    // =0..100 position du curseur de la couleur rouge des LEDs RGB
  int8_t slider_2;    // =0..100 position du curseur de la couleur bleue des LEDs RGB
  int8_t slider_3;    // =0..100 position du curseur de la couleur verte des LEDs RGB

  // autre variable
  uint8_t connect_flag; // =1 si le bluetooth est connecté, sinon =0

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#include "Tlc5940.h"

// Constantes du programme
#define NB_LED_CLIGNO 5 // nombre de séquences d'allumage des LEDs dans le mode clignotants
#define NB_LED_WARN 3   // nombre de séquences d'allumage des LEDs dans le mode warning
#define PUIS_MAX 4095   // valeur de l'intensité des sorties des modules Tlc5490 réglable de 0 à 4095
#define PUIS_BEEP 800   // volume sonore du bip accompagnant les clignotants et les warnings
#define PUIS_LEDS_RGB 8 // valeur réglable de 1 à 16
#define DELAI_FADE 30
#define DELAI_CLIGNO 100
#define DUREE_OFFWARNING 500 // temps d'extinction des warnings
#define DUREE_ONWARNING 500  // temps d'allumage des warnings
#define DUREE_OFFCLIGNO 500  // temps d'extinction des clignotants
#define DUREE_ONCLIGNO 500   // temps d'allumage des clignotants

// Renseignement entree/sorties
#define analogPin_cligno A0 // commande des clignotants sur l'entrée analogique A0
#define INPIN_KLAX A1       // commande du klaxon sur l'entrée analogique A1
#define INPIN_STOP A2       // bouton d'arrêt sur l'entrée analogique A2
#define analogPin_OVR A3    // commande OVR (Off, Ville, Route) sur l'entrée analogique A3
#define INPIN_PF A4         // commande des pleins phares sur l'entrée analogique A4
#define INPIN_WARN A5       // commande des warnings sur l'entrée analogique A5

void ledRVBpwm_1(int pwmRouge_1, int pwmVert_1, int pwmBleu_1); // paramètres pour les éclairages RGB des compteurs
void ledRVBpwm_2(int pwmRouge_2, int pwmVert_2, int pwmBleu_2); // paramètres pour les éclairages RGB du phare avant
void ledRVBpwm_3(int pwmRouge_3, int pwmVert_3, int pwmBleu_3); // paramètres pour les éclairages RGB de la plaque d'immatriculation

int val_cligno = 0;
int val_clef = 0;
int val_bpstop = 0;
int val_OVR = 0;
int valw = 0;
int etatBouton_PF = 0;
int etatBouton_STOP = 0;
int chut = 1;                      // valeur qui permet de couper le BIP des clignotants en mode R
int dimmer = 0;                    // valeur qui permet de réduire la luminosité des clignotants en mode R
int avg[5] = {4, 3, 2, 1, 0};      // ordre d'allumage des LEDs du clignotant avant gauche
int arg[5] = {15, 16, 17, 18, 19}; // ordre d'allumage des LEDs du clignotant arrière gauche
int avd[5] = {5, 6, 7, 8, 9};      // ordre d'allumage des LEDs du clignotant avant droit
int ard[5] = {14, 13, 12, 11, 10}; // ordre d'allumage des LEDs du clignotant arrière droit
int warning[3] = {2, 1, 0};
bool cmdWarning = false;
bool oldValw = false;
bool onWarning = false;
bool offWarning = false;
bool onClignoG = false;
bool offClignoG = false;
bool onClignoD = false;
bool offClignoD = false;
unsigned long timeOnWarning;
unsigned long timeOffWarning;
unsigned long timeOnCligno;
unsigned long timeOffCligno;
unsigned long time;
unsigned long previousMillis = 0;
unsigned long interval = 500;

// Séquence d'initialisation au démarrage

void setup()
{
  Serial.begin(9600);
  Serial.print("Hello\n");
  // RemoteXY_Init();

  // pinMode(2, OUTPUT);                // la sortie n°2 de l'ARDUINO alimente le relais de coupure moteur
  // pinMode(4, OUTPUT);                // la sortie n°4 de l'ARDUINO alimente le relais de maintien du contact à clé
  // pinMode(5, OUTPUT);                // la sortie n°5 de l'ARDUINO alimente le relais du klaxon
  // pinMode(6, OUTPUT);                // la sortie n°6 de l'ARDUINO alimente les feux de croisement
  // pinMode(7, OUTPUT);                // la sortie n°7 de l'ARDUINO alimente les pleins phares
  // pinMode(8, OUTPUT);                // la sortie n°8 de l'ARDUINO alimente le feu arrière
  // pinMode(INPIN_WARN, INPUT);        // détermine que la broche A5 est une entrée pour les warnings
  // pinMode(INPIN_KLAX, INPUT_PULLUP); // détermine que la broche A1 est l'entrée pour le bouton du klaxon

  // digitalWrite(2, HIGH); // activation du relais FINDER qui enlève la mise à la masse des 2xCDI
  // digitalWrite(4, HIGH); // activation du relais n°1 qui fait l'auto-maintien du contact à clé
  Tlc.init();
  Tlc.clear();
  Serial.print("Debut\n");
  // delay(3000);
  // Tlc.set(26, PUIS_MAX); // allumage en vert du tableau de bord
  // Tlc.set(27, 0);
  // Tlc.set(28, 0);
  // Tlc.set(23, PUIS_MAX); // allumage en vert du phare avant
  // Tlc.set(24, 0);
  // Tlc.set(25, 0);
  // Tlc.set(20, PUIS_MAX); // allumage en vert de la plaque d'immatriculation
  // Tlc.set(21, 0);
  // Tlc.set(22, 0);
  // Tlc.update();

  for (int ledIndex = 0; ledIndex < 20; ledIndex++)
  { // allumage en chenillard de toutes les LEDs
    Tlc.set(blinkersLedOrder[ledIndex], MAX_POWER);
    // Tlc.set(30, PUIS_MAX); // allumage du voyant vert de clignotant gauche
    // Tlc.set(31, PUIS_MAX); // allumage du voyant vert de clignotant droit
    Serial.print("Allumage LED : ");
    Serial.print(blinkersLedOrder[ledIndex], DEC);
    Serial.print("\n");
    Tlc.update();
    delay(200);
  }

  // Serial.print("Attente avant extinction\n");
  // delay(2000);

  // // // On peut donc lancer l'animation pour l'extinction douce des LEDs

  for (int power = MAX_POWER - FADE_OUT_STEP; power >= 0; power = (power - FADE_OUT_STEP) < 0 ? 0 : power - FADE_OUT_STEP)
  { // extinction de toutes les LEDs en même temps
    for (int ledIndex = 0; ledIndex < 20; ledIndex++)
    {
      Tlc.set(blinkersLedOrder[ledIndex], power);
      //     // Tlc.set(30, fadeValue); // extinction du voyant vert de clignotant gauche
      //     // Tlc.set(31, fadeValue); // extinction du voyant vert de clignotant droit
      //     // Tlc.set(26, fadeValue); // extinction du voyant vert du tableau de bord
      //     // Tlc.set(27, 0);
      //     // Tlc.set(28, 0);
      //     // Tlc.set(23, fadeValue); // extinction du voyant vert du phare avant
      //     // Tlc.set(24, 0);
      //     // Tlc.set(25, 0);
      //     // Tlc.set(20, fadeValue); // extinction du voyant vert de la plaque d'immatriculation
      //     // Tlc.set(21, 0);
      //     // Tlc.set(22, 0);
    }
    Tlc.update();
    delay(FADE_OUT_DELAY);
  }

  Serial.print("Attente de fin...\n");
  delay(5000);

  Tlc.clear();

  Serial.print("Fin\n");
}

// Boucle du programme principal

void loop()
{

  // // Récupération des entrées
  // RemoteXY_Handler();

  time = millis();
  val_cligno = analogRead(analogPin_cligno); // lecture de l'état de la commande des clignotants
  Serial.print("cligno = ");
  Serial.print(val_cligno, DEC);
  Serial.print("\n");
  // valw = digitalRead(INPIN_WARN); // lecture de l'état de la commande des warnings
  // if (valw == 0 && !oldValw)
  // {
  //   if (cmdWarning == false)
  //   {
  //     cmdWarning = true;
  //   }
  //   else
  //   {
  //     cmdWarning = false;
  //   }
  //   oldValw = true;
  // }
  // else if (valw == 1 && oldValw)
  // {
  //   oldValw = false;
  // }

  // etatBouton_PF = digitalRead(INPIN_PF); // lecture de l'état de la commande des pleins phares

  val_OVR = analogRead(analogPin_OVR); // lecture de l'état de la commande rotative OVR
  Serial.print("OVR = ");
  Serial.print(val_OVR, DEC);
  Serial.print("\n");

  delay(1000);
  // etatBouton_STOP = digitalRead(INPIN_STOP); // lecture de l'état de la commande d'arrêt du moteur

  /////////////////////////
  // CLIGNOTANTS GAUCHES //
  /////////////////////////

  // On regarde si le bouton Clignotants gauches nous intéresse
  // if (onClignoG == false && offClignoG == false)
  // {
  //   if (val_cligno < 400 || RemoteXY.turn_left == 1)
  //   { // S'il nous intéresse on lance une séquence
  //     timeOnCligno = millis();
  //     onClignoG = true;
  //     Serial.print("Cligno gauche\n")
  //   }
  // }
  // // Cas d'un allumage des clignotants gauches
  // if (onClignoG)
  // {
  //   if (millis() - timeOnCligno <= DUREE_ONCLIGNO)
  //   {
  //     int nbLedCligno = (int)((millis() - timeOnCligno) / (DUREE_ONCLIGNO / 5));
  //     for (int iCligno = 0; iCligno < nbLedCligno; iCligno++)
  //     { // lancement du scénario clignotants gauches
  //       Tlc.set(avg[iCligno], PUIS_MAX - dimmer);
  //       Tlc.set(arg[iCligno], PUIS_MAX - dimmer);
  //       Tlc.set(30, PUIS_MAX - dimmer); // allumage du voyant vert de clignotant gauche
  //       Tlc.set(29, PUIS_BEEP * chut);  // BIP ON

  //       Tlc.update();
  //     }
  //   }
  //   else
  //   {
  //     onClignoG = false;
  //     offClignoG = true;
  //     timeOffCligno = millis();
  //   }
  // }
  // // Cas d'une extinction des clignotants gauches
  // if (offClignoG)
  // {
  //   int fadeCligno = 0;
  //   if (millis() - timeOffCligno <= DUREE_OFFCLIGNO)
  //   {
  //     float t = millis() - timeOffCligno;
  //     fadeCligno = (PUIS_MAX - 45 - dimmer) * (float)(1 - ((float)(t / DUREE_OFFCLIGNO)));
  //     Tlc.set(0, fadeCligno);
  //     Tlc.set(1, fadeCligno);
  //     Tlc.set(2, fadeCligno);
  //     Tlc.set(3, fadeCligno);
  //     Tlc.set(4, fadeCligno);
  //     Tlc.set(15, fadeCligno);
  //     Tlc.set(16, fadeCligno);
  //     Tlc.set(17, fadeCligno);
  //     Tlc.set(18, fadeCligno);
  //     Tlc.set(19, fadeCligno);
  //     Tlc.set(30, fadeCligno); // extinction du voyant vert de clignotant gauche

  //     Tlc.set(29, 0); // BIP off

  //     Tlc.update();
  //   }
  //   else
  //   {
  //     fadeCligno = 0;
  //     Tlc.set(0, fadeCligno);
  //     Tlc.set(1, fadeCligno);
  //     Tlc.set(2, fadeCligno);
  //     Tlc.set(3, fadeCligno);
  //     Tlc.set(4, fadeCligno);
  //     Tlc.set(15, fadeCligno);
  //     Tlc.set(16, fadeCligno);
  //     Tlc.set(17, fadeCligno);
  //     Tlc.set(18, fadeCligno);
  //     Tlc.set(19, fadeCligno);
  //     Tlc.set(30, fadeCligno); // extinction du voyant vert de clignotant gauche

  //     Tlc.set(29, 0); // BIP off

  //     Tlc.update();

  //     offClignoG = false;
  //   }
  // }

  // /////////////////////////
  // // CLIGNOTANTS DROITS //
  // /////////////////////////

  // // On regarde si le bouton Clignotants droits nous intéresse
  // if (onClignoD == false && offClignoD == false)
  // {
  //   if (val_cligno < 600 && val_cligno > 400 || RemoteXY.turn_right == 1)
  //   { // S'il nous intéresse on lance une séquence
  //     timeOnCligno = millis();
  //     onClignoD = true;
  //   }
  // }
  // // Cas d'un allumage des clignotants droits
  // if (onClignoD)
  // {
  //   if (millis() - timeOnCligno <= DUREE_ONCLIGNO)
  //   {
  //     int nbLedCligno = (int)((millis() - timeOnCligno) / (DUREE_ONCLIGNO / 5));
  //     for (int iCligno = 0; iCligno < nbLedCligno; iCligno++)
  //     { // lancement du scénario clignotants droits
  //       Tlc.set(avd[iCligno], PUIS_MAX - dimmer);
  //       Tlc.set(ard[iCligno], PUIS_MAX - dimmer);
  //       Tlc.set(31, PUIS_MAX - dimmer); // allumage du voyant vert de clignotant droit
  //       Tlc.set(29, PUIS_BEEP * chut);  // BIP ON

  //       Tlc.update();
  //     }
  //   }
  //   else
  //   {
  //     onClignoD = false;
  //     offClignoD = true;
  //     timeOffCligno = millis();
  //   }
  // }
  // // Cas d'une extinction des clignotants droits
  // if (offClignoD)
  // {
  //   int fadeCligno = 0;
  //   if (millis() - timeOffCligno <= DUREE_OFFCLIGNO)
  //   {
  //     float t = millis() - timeOffCligno;
  //     fadeCligno = (PUIS_MAX - dimmer - 45) * (float)(1 - ((float)(t / DUREE_OFFCLIGNO)));
  //     Tlc.set(5, fadeCligno);
  //     Tlc.set(6, fadeCligno);
  //     Tlc.set(7, fadeCligno);
  //     Tlc.set(8, fadeCligno);
  //     Tlc.set(9, fadeCligno);
  //     Tlc.set(10, fadeCligno);
  //     Tlc.set(11, fadeCligno);
  //     Tlc.set(12, fadeCligno);
  //     Tlc.set(13, fadeCligno);
  //     Tlc.set(14, fadeCligno);
  //     Tlc.set(31, fadeCligno); // extinction du voyant vert de clignotant droit

  //     Tlc.set(29, 0); // BIP off

  //     Tlc.update();
  //   }
  //   else
  //   {
  //     fadeCligno = 0;
  //     Tlc.set(5, fadeCligno);
  //     Tlc.set(6, fadeCligno);
  //     Tlc.set(7, fadeCligno);
  //     Tlc.set(8, fadeCligno);
  //     Tlc.set(9, fadeCligno);
  //     Tlc.set(10, fadeCligno);
  //     Tlc.set(11, fadeCligno);
  //     Tlc.set(12, fadeCligno);
  //     Tlc.set(13, fadeCligno);
  //     Tlc.set(14, fadeCligno);
  //     Tlc.set(31, fadeCligno); // extinction du voyant vert de clignotant droit

  //     Tlc.set(29, 0); // BIP off

  //     Tlc.update();

  //     offClignoD = false;
  //   }
  // }

  // //////////////
  // // WARNINGS //
  // //////////////

  // // On regarde si le bouton Warning nous intéresse
  // if (onWarning == false && offWarning == false)
  // {
  //   if (cmdWarning || RemoteXY.warn == 1)
  //   { // S'il nous intéresse on lance une séquence
  //     timeOnWarning = millis();
  //     onWarning = true;
  //   }
  // }
  // // Cas d'un allumage des warning
  // if (onWarning)
  // {
  //   if (millis() - timeOnWarning <= DUREE_ONWARNING)
  //   {
  //     int nbLedWarning = (int)((millis() - timeOnWarning) / (DUREE_ONWARNING / 3));
  //     for (int iWarning = 0; iWarning < nbLedWarning; iWarning++)
  //     { // lancement du scénario Warning
  //       Tlc.set(avd[2 + iWarning], PUIS_MAX - dimmer);
  //       Tlc.set(ard[2 + iWarning], PUIS_MAX - dimmer);
  //       Tlc.set(avg[2 + iWarning], PUIS_MAX - dimmer);
  //       Tlc.set(arg[2 + iWarning], PUIS_MAX - dimmer);
  //       Tlc.set(avd[2 - iWarning], PUIS_MAX - dimmer);
  //       Tlc.set(ard[2 - iWarning], PUIS_MAX - dimmer);
  //       Tlc.set(avg[2 - iWarning], PUIS_MAX - dimmer);
  //       Tlc.set(arg[2 - iWarning], PUIS_MAX - dimmer);
  //       Tlc.set(30, PUIS_MAX - dimmer); // allumage du voyant vert de clignotant gauche
  //       Tlc.set(31, PUIS_MAX - dimmer); // allumage du voyant vert de clignotant droit
  //       Tlc.set(29, PUIS_BEEP * chut);  // BIP ON

  //       Tlc.update();
  //     }
  //   }
  //   else
  //   {
  //     onWarning = false;
  //     offWarning = true;
  //     timeOffWarning = millis();
  //   }
  // }
  // // Cas d'une extinction des warning
  // if (offWarning)
  // {
  //   int fadeWarning = 0;
  //   if (millis() - timeOffWarning <= DUREE_OFFWARNING)
  //   {
  //     float t = millis() - timeOffWarning;
  //     fadeWarning = (PUIS_MAX - dimmer - 45) * (float)(1 - ((float)(t / DUREE_OFFWARNING)));
  //     Tlc.set(0, fadeWarning);
  //     Tlc.set(1, fadeWarning);
  //     Tlc.set(2, fadeWarning);
  //     Tlc.set(2, fadeWarning);
  //     Tlc.set(2, fadeWarning);
  //     Tlc.set(2, fadeWarning);
  //     Tlc.set(2, fadeWarning);
  //     Tlc.set(3, fadeWarning);
  //     Tlc.set(4, fadeWarning);
  //     Tlc.set(5, fadeWarning);
  //     Tlc.set(6, fadeWarning);
  //     Tlc.set(7, fadeWarning);
  //     Tlc.set(8, fadeWarning);
  //     Tlc.set(9, fadeWarning);
  //     Tlc.set(10, fadeWarning);
  //     Tlc.set(11, fadeWarning);
  //     Tlc.set(12, fadeWarning);
  //     Tlc.set(13, fadeWarning);
  //     Tlc.set(14, fadeWarning);
  //     Tlc.set(15, fadeWarning);
  //     Tlc.set(16, fadeWarning);
  //     Tlc.set(17, fadeWarning);
  //     Tlc.set(18, fadeWarning);
  //     Tlc.set(19, fadeWarning);
  //     Tlc.set(30, fadeWarning); // extinction du voyant vert de clignotant gauche
  //     Tlc.set(31, fadeWarning); // extinction du voyant vert de clignotant droit

  //     Tlc.set(29, 0); // BIP off

  //     Tlc.update();
  //   }
  //   else
  //   {
  //     fadeWarning = 0;
  //     Tlc.set(0, fadeWarning);
  //     Tlc.set(1, fadeWarning);
  //     Tlc.set(2, fadeWarning);
  //     Tlc.set(3, fadeWarning);
  //     Tlc.set(4, fadeWarning);
  //     Tlc.set(5, fadeWarning);
  //     Tlc.set(6, fadeWarning);
  //     Tlc.set(7, fadeWarning);
  //     Tlc.set(8, fadeWarning);
  //     Tlc.set(9, fadeWarning);
  //     Tlc.set(10, fadeWarning);
  //     Tlc.set(11, fadeWarning);
  //     Tlc.set(12, fadeWarning);
  //     Tlc.set(13, fadeWarning);
  //     Tlc.set(14, fadeWarning);
  //     Tlc.set(15, fadeWarning);
  //     Tlc.set(16, fadeWarning);
  //     Tlc.set(17, fadeWarning);
  //     Tlc.set(18, fadeWarning);
  //     Tlc.set(19, fadeWarning);
  //     Tlc.set(30, fadeWarning); // extinction du voyant vert de clignotant gauche
  //     Tlc.set(31, fadeWarning); // extinction du voyant vert de clignotant droit

  //     Tlc.set(29, 0); // BIP off

  //     Tlc.update();

  //     offWarning = false;
  //   }
  // }
  // // Scénario pour l'activation du Klaxon

  // boolean etatBouton_klax = digitalRead(INPIN_KLAX);
  // if (etatBouton_klax == 0 || RemoteXY.klaxon == 1)
  // {
  //   digitalWrite(5, HIGH);
  // }
  // else if (etatBouton_klax == 1 || RemoteXY.klaxon == 0)
  // {
  //   digitalWrite(5, LOW);
  // }

  /////////////////
  //     OVR     //
  /////////////////

  // Scénario de commande du commutateur rotatif OVR (Off, Ville, Route)

  // if (val_OVR < 600 && val_OVR > 300 || RemoteXY.OVR == 1)
  // {
  //   Tlc.set(20, PUIS_MAX);
  //   Tlc.set(21, PUIS_MAX);
  //   Tlc.set(22, PUIS_MAX);
  //   Tlc.set(23, PUIS_MAX);
  //   Tlc.set(24, PUIS_MAX);
  //   Tlc.set(25, PUIS_MAX);
  //   Tlc.set(26, PUIS_MAX);
  //   Tlc.set(27, PUIS_MAX);
  //   Tlc.set(28, PUIS_MAX);
  //   Tlc.update();
  //   digitalWrite(6, LOW);
  //   digitalWrite(8, LOW);
  //   chut = 1;
  //   dimmer = 0;
  // }
  // else if (val_OVR > 600 || RemoteXY.OVR == 2)
  // {
  //   Tlc.set(20, PUIS_MAX / 3);
  //   Tlc.set(21, PUIS_MAX / 3);
  //   Tlc.set(22, PUIS_MAX / 1);
  //   Tlc.set(23, PUIS_MAX / 3);
  //   Tlc.set(24, PUIS_MAX / 3);
  //   Tlc.set(25, PUIS_MAX / 1);
  //   Tlc.set(26, PUIS_MAX / 4);
  //   Tlc.set(27, PUIS_MAX / 2);
  //   Tlc.set(28, PUIS_MAX / 4);
  //   Tlc.update();
  //   digitalWrite(6, HIGH);
  //   digitalWrite(8, HIGH);
  //   chut = 0;
  //   dimmer = 2500;
  // }
  // else
  // {
  //   Tlc.set(20, 0);
  //   Tlc.set(21, 0);
  //   Tlc.set(22, 0);
  //   Tlc.set(23, 0);
  //   Tlc.set(24, 0);
  //   Tlc.set(25, 0);
  //   Tlc.set(26, 0);
  //   Tlc.set(27, 0);
  //   Tlc.set(28, 0);
  //   Tlc.update();
  //   digitalWrite(6, LOW);
  //   digitalWrite(8, LOW);
  //   chut = 1;
  //   dimmer = 0;
  // }

  // // Scénario pour l'activation des pleins phares
  // if (etatBouton_PF == 0 && val_OVR < 600 || RemoteXY.light == 1)
  // { // allumage des pleins phares en mode O ou V
  //   digitalWrite(6, HIGH);
  //   digitalWrite(7, HIGH);
  // }
  // else if (etatBouton_PF == 0 || RemoteXY.light == 1)
  // { // allumage des pleins phares en mode R
  //   digitalWrite(7, HIGH);
  // }
  // else
  // {
  //   digitalWrite(7, LOW); // extinction des pleins phares
  // }

  // // Gestion des bandeaux de LEDs RGB
  // //  Scénario de variation des LEDs RGB - COMPTEURS
  // //  int val_1 = map(RemoteXY.slider_1,0,100,0,2*255);
  // //  if( val_1 < 255)
  // //    ledRVBpwm(0, val_1, 255-val_1, 26);
  // //  if( val_1 >= 255 && val_1 <= 2*255)
  // //    ledRVBpwm(val_1-255, 255-(val_1-255), 0, 26);

  // // Scénario de variation des LEDs RGB - PHARE AVANT
  // // int val_2 = map(analogRead(A2),0,1023,0,2*255);
  // // if( val_2 < 255)
  // //   ledRVBpwm(0, val_2, 255-val_2, 23);
  // // if( val_2 >= 255 && val_2 <= 2*255)
  // //   ledRVBpwm(val_2-255, 255-(val_2-255), 0, 23);

  // // Scénario de variation des LEDs RGB - PLAQUE D'IMMATRICULATION
  // // int val_3 = map(analogRead(A3),0,1023,0,2*255);
  // // if( val_3 < 255)
  // //   ledRVBpwm(0, val_3, 255-val_3, 20);
  // // if( val_3 >= 255 && val_3 <= 2*255)
  // //   ledRVBpwm(val_3-255, 255-(val_3-255), 0, 20);
  // // Fin gestion des LEDs

  // /////////////////////////////////////
  // // SCENARIO D'EXTINCTION DU MOTEUR //
  // /////////////////////////////////////

  // if (etatBouton_STOP == 0 || RemoteXY.moteur == 1)
  // {
  //   digitalWrite(2, LOW); // mise à la masse des boîtiers CDI et arrêt du moteur
  //   delay(2000);
  //   for (int fadeValue = 0; fadeValue <= PUIS_MAX - 45; fadeValue = fadeValue + 150)
  //   {
  //     for (int i = 0; i < 20; i++)
  //     {
  //       Tlc.set(i, fadeValue);  // allumage de toutes les LEDs des clignotants
  //       Tlc.set(30, fadeValue); // allumage du voyant vert de clignotant gauche
  //       Tlc.set(31, fadeValue); // allumage du voyant vert de clignotant droit
  //       Tlc.set(28, fadeValue); // allumage du voyant rouge du tableau de bord
  //       Tlc.set(27, 0);
  //       Tlc.set(26, 0);
  //       Tlc.set(25, fadeValue); // allumage du voyant rouge du phare avant
  //       Tlc.set(24, 0);
  //       Tlc.set(23, 0);
  //       Tlc.set(22, fadeValue); // allumage du voyant rouge de la plaque d'immatriculation
  //       Tlc.set(21, 0);
  //       Tlc.set(20, 0);
  //     }
  //     Tlc.update();
  //     delay(DELAI_FADE);
  //   }
  //   delay(500);
  //   for (int led = 0; led <= 20; led++)
  //   { // extinction en chenillard de toutes les LEDs des clignotants
  //     Tlc.set(led, 0);
  //     Tlc.set(30, 0);
  //     Tlc.set(31, 0);
  //     Tlc.set(22, 0);
  //     Tlc.set(25, 0);
  //     Tlc.set(28, 0);
  //     Tlc.update();
  //     delay(70);
  //   }
  //   digitalWrite(4, LOW); // désactivation du relais n°1 qui fait l'auto-maintien du contact à clé
  //   delay(2000);
  //   Tlc.clear();
  // }
  // else if (etatBouton_STOP == 1 || RemoteXY.moteur == 0)
  // { // le moteur tourne OK
  //   digitalWrite(2, HIGH);
  // }
  // else
  // {
  //   digitalWrite(2, HIGH); // si rien de particulier, on laisse le moteur tourner
  // }
}

void ledRVBpwm(int pwmRouge_1, int pwmVert_1, int pwmBleu_1, int portDepart)
{ // reçoit valeur 0-255 par couleur
  // Tlc.set(portDepart+2,pwmRouge_1*PUIS_LEDS_RGB); // LEDs Rouges
  // Tlc.set(portDepart,pwmVert_1*PUIS_LEDS_RGB); // LEDs Vertes
  // Tlc.set(portDepart+1,pwmBleu_1*PUIS_LEDS_RGB); // // LEDs Bleues
  //  Tlc.set(26,RemoteXY.slider_1*40);
  //  Tlc.set(27,RemoteXY.slider_2*40);
  //  Tlc.set(28,RemoteXY.slider_3*40);
  Tlc.update();
}