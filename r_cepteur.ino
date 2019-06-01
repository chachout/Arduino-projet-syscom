int pinRecepteur = 5; // variable de broche
unsigned long dureehaut;  // variable utilisée pour stocker la durée
int ledRouge = 0;
int ledVerte = 0;
const int pinLedRouge = 8;
const int pinLedVerte = 7;

void setup()
{
  pinMode(pinRecepteur, INPUT); // met la broche en entrée
  Serial.begin(9600);

  pinMode(pinLedRouge, OUTPUT);
  digitalWrite(pinLedRouge,LOW); 

  pinMode(pinLedVerte, OUTPUT);
  digitalWrite(pinLedVerte,LOW); 

}

void loop()
{
  int code[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ;
  for (int i=0;i<16;i++)
  {
    digitalWrite(pinRecepteur,HIGH);
    dureehaut = pulseIn(pinRecepteur, HIGH); // met la durée de l'impulsion de niveau HAUT dans la variable duree
    
    if(abs(dureehaut-10000)<1000) 
      {
        code[i]=1;
        delay((13000-dureehaut)/1000);
        digitalWrite(pinLedVerte,HIGH);
      }
     else 
      {
        if(abs(dureehaut-3000)<1000) 
        {
          code[i]=0;
          delay((13000-dureehaut)/1000);
          digitalWrite(pinLedRouge,HIGH);
      }
      digitalWrite(pinLedRouge,LOW);
      digitalWrite(pinLedVerte,LOW);
  }
  
  }
}
 

