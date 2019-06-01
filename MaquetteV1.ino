/* Mise en oeuvre des éléments suivants :
  - d'un détecteur optique
  - d'un servomoteur
  - d'un capteur ultrason
  - d'un bouton poussoir 
  - d'un emetteur RF 433 MHz

   Fonctionnements : 
A la détection d'un changement d'état sur le fourche optique (coupure du faisceau, libération du faisceau (utilisation d'une interruption))
le servo moteur s'actionne alternativement dans un sens ou dans l'autre. Lorsqu'il est en fonctionnement, la led rouge s'allume.

Lorsqu'une détection (de proximité) ultrason est réalisée, la led verte s'allume.

Lorsque l'interrupteur est appuyé (utilisation d'une interruption), un code est envoyé 4 fois sur la bande des 433 MHz. 
Lorsque le code est envoyé, la led bleue s'allume.
*/

// pour utilisation du servomoteur
#include <Servo.h>

// Déclarations
///////////////
//création d'un nouveau servomoteur
Servo monServo;
const int pinMonServo = 4;
int angle;
int testDepartGarage=0;

// fourche optique
const int pinFourche = 2; // (interruption sur pin 2)
// info led détection fourche
int ledRouge = 0;
const int pinLedRouge = 8;

// capteur ultrason
const int pinTriggerUltrason=6;
const int pinEchoUltrason=7;
const unsigned long MEASURE_TIMEOUT = 2900UL; // temps correspondant à environ 10 cm à 340m/s
const float SOUND_SPEED = 340.0 / 1000;
const int DistanceMax = 10; // fixe la distance maximale de détection
boolean detection; // résultat de détection
const int pinLedVerte = 9;


// bouton 
const int pinInterupteur = 3; // (interruption sur pin 3)
const int pinLedBleue = 10;
int ledBleue = 0;


// Emetteur 433 MHz
const int pinEmetteur = 5;
const char codebouton[]="1001001001001001"; //  code à  envoyer
//const char codebouton[]="0000000000"; //  code à  envoyer
const int nbrepet=4; // nbre de répétition du code
const int tempsHaut1=10; // durée ms niveau haut pour un 1
const int tempsBas1=3; // durée ms niveau bas pour un 1
const int tempsHaut0=3; // durée ms niveau haut pour un 0
const int tempsBas0=10; // durée ms niveau bas pour un 0
const int tempsEntreRepetition=200; // durée ms niveau bas pour un 0
// Initialisations
///////////////
void setup()
{
    //on déclare l'entrée du servo connectée sur la broche 2
    monServo.attach(pinMonServo);
    // garage fermé au début
    monServo.write(20);
    
    // on déclare l'interuption détecteur fourche pour exécuter la fonction allumerLedRouge sur chaque changement
    attachInterrupt(digitalPinToInterrupt(pinFourche), allumerLedRouge, CHANGE );
    // on déclare la sortie led Rouge : elle s'allumera si le moteur fonctionne
    pinMode(pinLedRouge, OUTPUT);
    digitalWrite(pinLedRouge,LOW); 
    
    // on déclare la sortie led verte : elle s'allumera si on détecte en ultrason
    pinMode(pinLedVerte, OUTPUT);
    digitalWrite(pinLedVerte,LOW);

    // détecteur ultrason
    pinMode(pinTriggerUltrason,OUTPUT);
    digitalWrite(pinTriggerUltrason, LOW); // mise à 0 à l'initialisation
    pinMode(pinEchoUltrason,INPUT);

    // on déclare l'interuption détecteur fourche pour exécuter la fonction allumerLedRouge sur chaque changement
    attachInterrupt(digitalPinToInterrupt(pinInterupteur), allumerLedBleue, CHANGE );
    // on déclare la sortie led Bleue : elle s'allumera si le moteur fonctionne
    pinMode(pinLedBleue, OUTPUT);
    digitalWrite(pinLedBleue,HIGH);
    // on déclare la sortie émetteur
    pinMode(pinEmetteur, OUTPUT);
    
     
    //on oublie pas de démarrer la liaison série ;-)
    Serial.begin(9600); 
}

// Boucle sur instructions
///////////////
void loop() {
  // 

  
  // test interruption sur capteur ultrason et allumage led verte
  detection = detectionUltrason(DistanceMax);
  if(detection){
    digitalWrite(pinLedVerte,HIGH);
  }
  else {
    digitalWrite(pinLedVerte,LOW);
  }

  // test interruption sur capteur optique et allumage led rouge
  if(testDepartGarage == 1) {
    if(ledRouge==0) {
      digitalWrite(pinLedRouge,HIGH);
      angleModif(160); // ouverture garage
      digitalWrite(pinLedRouge,LOW);
      testDepartGarage=0;
    }
    else {
      digitalWrite(pinLedRouge,HIGH);
      angleModif(20); // fermeture garage
      digitalWrite(pinLedRouge,LOW);
      testDepartGarage=0;
    }
  }

  // test interuption sur poussoir
  if(ledBleue==1) {
    Emission(codebouton); // émission du code
  }
  else {
    digitalWrite(pinLedBleue,LOW);
  }
  delay(100);
}


// fonctions de gestion des angles du servomoteur
void angleModif(int angleAttendu)
{
  if(angle < angleAttendu) {
    // on incrémente l'angle
    anglePlus(angleAttendu);
  }
  else if(angle > angleAttendu) { 
    // on décrémente l'angle
    angleMoins(angleAttendu);
  }
}
void anglePlus(int angleAttendu)
{
  while(angle < angleAttendu)
  {
    monServo.write(angle++);
    delay(10);
    }
}
void angleMoins(int angleAttendu)
{
  while(angle > angleAttendu)
    {
      monServo.write(angle--);
      delay(10);
    }
}


// fonction d'interruption sur faisceau
// interuption sur changement : si cela change, on teste la valeur courante, ce qui permet
//   d'éviter les rebonds visuellement
void allumerLedRouge() 
{
  testDepartGarage=1;
  if(digitalRead(pinFourche)==1){
    ledRouge=0;
  }
  else {
    ledRouge=1;
  }
}

// fonction d'interruption sur poussoir
// interuption sur changement : si cela change, on teste la valeur courante, ce qui permet
//   d'éviter les rebonds visuellement
void allumerLedBleue() {
  ledBleue=digitalRead(pinInterupteur)==1 ? 1:0;
}

// fonction détection par ultrason
boolean detectionUltrason(int DMax) 
{
// TRIGGER PULSE
  boolean detect=false;
  // envoi d'un pulse
  digitalWrite(pinTriggerUltrason, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTriggerUltrason, LOW);
  // écoute de l'écho
  long measure = pulseIn(pinEchoUltrason, HIGH, MEASURE_TIMEOUT);
  //Serial.print("mesure de temps : ");
  //Serial.println(measure);
  float distance_mm = measure / 2.0 * SOUND_SPEED;
  delay(100);
        
  if(distance_mm >10 && distance_mm < DMax*10) {
    detect = true;
  }
  return detect;       
}

// fonction qui permet d'émettre des niveaux vers l'emetteur 433 MHz
void Emission(const char* code) 
{
  // code de 10 digits obligatoirement
  for(int j=0;j<nbrepet;j++) { // boucle pour nbrepet répétitions de l'émission
    for(int i=0;i<16;i++) { // boucle sur les 10 digits du code
      if(code[i]=='1') {
        // émission d'un "1"
        digitalWrite(pinEmetteur,HIGH);
        digitalWrite(pinLedBleue,HIGH);
        delay(tempsHaut1); // niveau HIGH pendant tempsHaut1 ms
        digitalWrite(pinEmetteur,LOW);
        digitalWrite(pinLedBleue,LOW);
        delay(tempsBas1); // niveau HIGH pendant tempsBas1 ms
      }
      else { // émission d'un "0"
        digitalWrite(pinEmetteur,HIGH);
        digitalWrite(pinLedBleue,HIGH);
        delay(tempsHaut0); // niveau HIGH pendant tempsHaut0 ms
        digitalWrite(pinEmetteur,LOW);
        digitalWrite(pinLedBleue,LOW);
        delay(tempsBas0); // niveau HIGH pendant tempsBas0 ms
      }
    }
    delay(tempsEntreRepetition); // attente tempsEntreRepetition ms entre deux émissions du code complet
  }
}


