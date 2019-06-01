
/*
RF Blink - Receiver sketch
Written by ScottC 17 Jun 2014
Arduino IDE version 1.0.5
Website: http://arduinobasics.blogspot.com
Receiver: XY-MK-5V
Description: A simple sketch used to test RF transmission/receiver.
------------------------------------------------------------- */
#define rfReceivePin A5 //RF Receiver pin = Analog pin 5
#define ledPinV 7 //Onboard LED = digital pin 7
#define ledPinR 8

unsigned int data = 0; // variable used to store received data
const unsigned int upperThreshold = 700; //upper threshold value
const unsigned int lowerThreshold = 50; //lower threshold value
int code[]={0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1} ;
int ledRouge = 0;
int ledVerte = 0;

void setup()
{
  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinV, OUTPUT);
  digitalWrite(ledPinR,LOW); 
  Serial.begin(9600);
  digitalWrite(ledPinV,LOW); 
}
void loop()
{
  for (int i=0;i<16;i++)
  {
    digitalWrite(rfReceivePin,HIGH);
    data=analogRead(rfReceivePin); //listen for data on Analog pin 0
    if(abs(pulseIn(data,HIGH)-10000)>1000)
    {
      digitalWrite(ledPinR, HIGH); //If a LOW signal is received, turn LED OFF
      code[i]=1;
      Serial.println(1);
      digitalWrite(ledPinR, LOW);
    }
    if(abs(pulseIn(data,HIGH)-3000)>1000)
    {
      digitalWrite(ledPinV, HIGH); //If a HIGH signal is received, turn LED ON
      code[i]=0;
      Serial.println(0);
      digitalWrite(ledPinV, LOW);
    }
  }
    delay (200);
    for (int j=0;j<16;j++)
    {
      if (code[j]==1)
        {
          delay(300);
          digitalWrite(ledPinV,HIGH);
          delay(300);
        }
      else 
      {
         if (code[j]==0)
        {
          delay(300);
          digitalWrite(ledPinR,HIGH);
          delay(300);
        }
      }
      digitalWrite(ledPinR,LOW);
      digitalWrite(ledPinV,LOW);
  }
  delay(3000);
}





