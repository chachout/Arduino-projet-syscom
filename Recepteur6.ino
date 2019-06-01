// pour utilisation du servomoteur
#include <Servo.h>

// Declaration des constantes
int pinRecepteur = 5; // Variable de broche
int ledRouge = 0; 
int ledVerte = 0;
const int pinLedVerte = 9; // Broche 9 pour la led verte
const int pinLedRouge = 8; // Broche 8 pour la led rouge 
unsigned long dHaut; // On va stocker notre pulseIn et donc le temps Haut de l'impulsion 
int codeRecu[]={0,0,0,0,0,0,0,0,0,0}; // On initalise notre code 
int codeEmi[]={1,0,1,0,0,0,0,1,0,1};
int k=0;
int Egalite=7;
//int Egalite[]={7,7,7,7,7,7,7,7,7,7};
//int sécurite=0;
//int cpt=0;

// LE SERVO
Servo monServo;
const int pinMonServo = 4;
int angle;

void setup()
{
     //on déclare l'entrée du servo connectée sur la broche 2
  monServo.attach(pinMonServo);
    // garage fermé au début
  monServo.write(20);
  pinMode(pinLedRouge,OUTPUT);
  digitalWrite(pinLedRouge,LOW);
  pinMode(pinLedVerte,OUTPUT);
  digitalWrite(pinLedVerte,LOW);
  pinMode(pinRecepteur, INPUT); 
  Serial.begin(9600); 
}

void loop()
{
  dHaut=pulseIn(pinRecepteur,HIGH);
  if (dHaut>500)
  {
    if (dHaut>9000)
    {
      Serial.print(1);
      codeRecu[k]=1;
      if (codeEmi[k]!=1)
      {
        Egalite=6;
        
        
      }
      k=k+1;
    }
    else 
    {
      Serial.print(0);
      codeRecu[k]=0;
      if (codeEmi[k]!=0)
      {
        Egalite=6;
        
      }
      k=k+1;
    }
//    Serial.print(Egalite);
  }
 
}
// fonctions de gestion des angles du servomoteur
//void angleModif(int angleAttendu)
//{
//void test()
//{
//  if (k==9)
//  {
//    if (Egalite==7)
//    {
//      Serial.print(8);
//    }
//  }
//}
//  if(angle < angleAttendu) {
//    // on incrémente l'angle
//    anglePlus(angleAttendu);
//  }
//  else if(angle > angleAttendu) { 
//    // on décrémente l'angle
//    angleMoins(angleAttendu);
//  }
//}
//void anglePlus(int angleAttendu)
//{
//  while(angle < angleAttendu)
//  {
//    monServo.write(angle++);
//    delay(10);
//    }
//}
//void angleMoins(int angleAttendu)
//{
//  while(angle > angleAttendu)
//    {
//      monServo.write(angle--);
//      delay(10);
//    }
//}
