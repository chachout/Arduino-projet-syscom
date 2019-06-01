
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


void setup()
{

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
  if (dHaut>3000)
  {
    digitalWrite(pinLedVerte,LOW);
    if (dHaut>9000)
    {
      Serial.print(1);
      codeRecu[k]=1;
      k=k+1;
    }
    else 
    {
      Serial.print(0);
      codeRecu[k]=0;
      k=k+1;
    }
  }
 
}





