int pinRecepteur = 5; // variable de broche
unsigned long dureehaut;  // variable utilisée pour stocker la durée
int ledRouge = 0;
int ledVerte = 0;
const int pinLedRouge = 8;
const int pinLedVerte = 7;
int code[]={0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1} ;

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
  for (int i=0;i<16;i++)
  {
    digitalWrite(pinRecepteur,HIGH);
    dureehaut = pulseIn(pinRecepteur, HIGH); // met la durée de l'impulsion de niveau HAUT dans la variable duree
      
    if(abs(dureehaut-10000)<1000)
      {
        code[i]=1;
        Serial.println(1);
        delay((13000-dureehaut)/1000);
      }
     else 
      {
        if(abs(dureehaut-3000)<1000) 
        {
          code[i]=0;
          Serial.println(0);
          delay((13000-dureehaut)/1000);
        }
      }
      
  }
  delay (200);
  for (int j=0;j<16;j++)
  {
    if (code[j]==1)
      {
        delay(300);
        digitalWrite(pinLedVerte,HIGH);
        delay(300);
      }
     else 
      {
        if (code[j]==0)
        {
          delay(300);
          digitalWrite(pinLedRouge,HIGH);
          delay(300);
        }
      }
      digitalWrite(pinLedRouge,LOW);
      digitalWrite(pinLedVerte,LOW);
  }
  delay(3000);
  }
  

 

