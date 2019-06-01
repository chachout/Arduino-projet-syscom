int x;
void setup() {
  
  Serial.begin(9600);        // On initie la communication sÃ©rie sur le dÃ©bit de 9600 bauds
  pinMode(5, INPUT);
}

void loop() {
  int n=0;
  do{
    x=digitalRead(5);
    n=n+1;
    delay(1);
  }while(x==1);
  if (n==9 or n==10 or n==11 or n==12 or n==13 or n==14 or n==15 or n==16){
    Serial.println("1");
  }
  else if (n==2 or n==3 or n==4 or n==5 or n==6){
   Serial.println("0");
  }
  
}

