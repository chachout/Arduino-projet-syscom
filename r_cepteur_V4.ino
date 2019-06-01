+int pinRecepteur = 5; // variable de broche
unsigned long dureehaut;  // variable utilisée pour stocker la durée
int ledRouge = 0;
int ledVerte = 0;
const int pinLedRouge = 8;
const int pinLedVerte = 7;
int code[]={0,1,1,0,0,1,1,0,0,1} ;
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
  for (int k=0;k<3;k++)
  for(int h=13;h<16;h++)
      {
      {
      digitalWrite(pinRecepteur,HIGH);
      dureehaut = pulseIn(pinRecepteur, HIGH); // met la durée de l'impulsion de niveau HAUT dans la variable duree
      if(abs(dureehaut-15000)<3000)
        {
          code[k]=1;
          delay((20000-dureehaut)/1000);
        }
       else 
        {
        if(abs(dureehaut-5000)<3000) 
          {
            code[k]=0;
            delay((20000-dureehaut)/1000);
          }
        }
        delay(130);
        if(abs(dureehaut-15000)<3000)
          {
            code[h]=1;
            delay((20000-dureehaut)/1000);
          }
        else 
          {
          if(abs(dureehaut-5000)<3000) 
            {
            code[h]=0;
            delay((20000-dureehaut)/1000);
            }
          }
       if ((code [0]==1) & (code [1]==0) & (code [2]==0) & (code[10]==0) & (code[11]==0) & (code[12]==1))
          {
          delay(200);
          for (int i=0;i<13;i++)
            {
            if(abs(dureehaut-10000)<3000)
              {
              code[i]=1;
              delay((13000-dureehaut)/1000);
              }
            else 
              {
              if(abs(dureehaut-3000)<3000) 
                {
                code[i]=0;
                delay((13000-dureehaut)/1000);
                }
              }
             }
             delay (200);
          }
       }
      }


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
