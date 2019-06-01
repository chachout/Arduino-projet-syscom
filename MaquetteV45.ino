#define rfTransmitPin 12  //RF Transmitter pin = digital pin 12
#define ledPin 13        //Onboard LED = digital pin 13


 void setup(){
   pinMode(rfTransmitPin, OUTPUT);     
   pinMode(ledPin, OUTPUT);
   Serial.begin(250000);    // taux de transfert (bauds)
  //Correspond à artie 1: Saisie et découpage du message:
   Serial.println("Saisissez votre message (au maximum 16 charactres) ");
   Serial.print("Vous avez saisi: ");
   
   
 }
 
 
 
 
 void loop(){


   //*****************************************************************\\
   //********** Conversion message en bits et encapsulation **********\\
   //*****************************************************************\\
   
   
   //Partie 1: Saisie et découpage du message:

   int i=0;
   char motDecoupe[16];

   //j'efface le tableau motDecoupe car il y a quelques pb sinon 
   motDecoupe[0] = 0;
   for(int reset=1; reset<16; reset++) {
     motDecoupe[reset] = ' ';
   }
   
   int lettreBIN[7];
   char lettretemp;

   while (Serial.available()) { // tant que des caractères sont en attente d'être lus
    
    char lettreTemporaire = Serial.read(); // on lit le charactère
    motDecoupe[i] = lettreTemporaire; // on complète le tableau charactère par charactère
    i++;  // à la fin i représentera le nombre de charactères du message!
    Serial.println(lettreTemporaire); // on l'écrit sur le moniteur   
    delay(10); // petit temps de pause
   }

   //modif 
   //**************************************************************
   if(i>0)
   {
      for(i; i<16; i++)
      {
          char espace=' ';
          motDecoupe[i]=espace;
      }
   
      Serial.println();
   
      for(i=0;i<16;i++)
      {
        Serial.print(motDecoupe[i]);
      }
   }   
  // *************************************************************
   
   //Partie 3: Conversion en binaire et encapsulation puis envoi:

   //tant qu'on a rien rentré on y entre pas, sinon on répète l'envoi en boucle
   if(motDecoupe[0] != 0){
   while(true) {
   Serial.println("Syncro");
   // Debut adresse: 11001001
   EnvoiUn();                
   EnvoiUn();
   EnvoiZero();
   EnvoiZero();
   EnvoiUn();
   EnvoiZero();
   EnvoiZero();
   EnvoiUn();
   //Fin adresse



   
   //Envoi lettres
   for(int k=0; k<i; k++) { 
    Serial.print(motDecoupe[k]);     
    //lettre -> tableau de 7bits complété ci dessous par une synchro de 1 bit = 1 ci dessous
   
       EnvoiZero();

    //lettre -> tableau de 7bits
    
      lettretemp = motDecoupe[k];
      for(int l=0; l<7; l++) {
        lettreBIN[6-l] = lettretemp & 1;  // prend le LSB et le sauve dans nombre
        lettretemp = lettretemp >> 1;    // décalage d'un bit sur la droite
  
    }

     //envoi d'une lettre
     for(int l=0; l<7; l++) {
      Serial.print(lettreBIN[l]);    
      if(lettreBIN[l] == 0) {
       EnvoiZero();
      }

      if(lettreBIN[l] == 1) {
       EnvoiUn();
      }
     }
     Serial.println();


    
   }
   //Fin envoi lettres

   }
   }
 }

 void EnvoiZero(){
   digitalWrite(rfTransmitPin, LOW);     //Transmit a HIGH signal
   digitalWrite(ledPin, LOW);            //Turn the LED on
   delay(20);                           //Wait for 1 milisecond
   digitalWrite(rfTransmitPin, LOW);     //Transmit a HIGH signal
   digitalWrite(ledPin, LOW);            //Turn the LED on
   delay(20);         
   
 }

  void EnvoiUn(){
   digitalWrite(rfTransmitPin, HIGH);     //Transmit a HIGH signal
   digitalWrite(ledPin, HIGH);            //Turn the LED on
   delay(20);                           //Wait for 1 milisecond
   digitalWrite(rfTransmitPin, LOW);     //Transmit a HIGH signal
   digitalWrite(ledPin, LOW);            //Turn the LED on
   delay(20); 
 }
   


