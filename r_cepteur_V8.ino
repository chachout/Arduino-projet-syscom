// pour utilisation du servomoteur
#include <Servo.h>

// Declaration des constantes
int pinRecepteur = 5; // Variable de broche
int ledRouge = 0; 
int ledVerte = 0;
const int pinLedVerte = 7; // Broche 7 pour la led verte
const int pinLedRouge = 8; // Broche 8 pour la led rouge 
unsigned long dHaut; // On va stocker notre pulseIn et donc le temps Haut de l'impulsion 
int codeRecu[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // On initalise notre code 
int codeEmi[]={1,0,0,1,1,1,1,1,1,1,0,0,1};
int k=0;

// LE SERVO
Servo monServo;
const int pinMonServo = 4;
int angle;

void setup()
{
  // on déclare la sortie led Rouge : elle s'allumera si le récepteur reçoit 0
  pinMode(pinLedRouge,OUTPUT);
  digitalWrite(pinLedRouge,LOW);
  // on déclare la sortie led Verte : elle s'allumera si le récepteur reçoit 1
  pinMode(pinLedVerte,OUTPUT);
  digitalWrite(pinLedVerte,LOW);
  pinMode(pinRecepteur, INPUT); 
  Serial.begin(9600); 
}

void loop()
{
  dHaut=pulseIn(pinRecepteur,HIGH);//récupération du signal
  if (dHaut>2500) //condition pour éliminer valeur trop basse
  {
    if (dHaut>9000)// condition pour récupérer les 1
    {
      Serial.print(1); // affichage du 1 dans le monitor série
      codeRecu[k]=1; // enregistrement du 1
      
      k=k+1;
    }
    else 
    {
     
      Serial.print(0);// affichage du 0 dans le monitor série
      codeRecu[k]=0;// enregistrement du 0
      k=k+1;
       
    }
  }
} 
