#define rfReceivePin A0             //RF Receiver pin = Analog pin 0
#define ledPin 13                   //Onboard LED = digital pin 13
#include <Wire.h>                   //LCDLibrary
#include <LiquidCrystal_I2C.h>      //?????
LiquidCrystal_I2C lcd(0x20, 16, 2); //?????

//VARIABLES*******************************************************************************************************************************
int affichage = 0;
int i = 0, j = 0, l = 0, w = 0, v = 0, result, testsyncro, intBit,cptlcd = 0, o = 0;;
int compt = 0, bit0[40];
boolean End = LOW, donnee[50], wait40 = LOW, wait10 = LOW, syncro = LOW, mot[128], lettre[8];
boolean syncrotab[8] = {HIGH, HIGH, LOW, LOW, HIGH, LOW, LOW, HIGH};                            //Tableau de la Syncro
double bit1[100];                                                                               //Signal Filtré
char charBit, motf[16];
unsigned int data = 0;                                                                          //Variable pour recevoir les données
const unsigned int upperThreshold = 350;                                                        //Valeur Seuil Haut
const unsigned int lowerThreshold = 200;                                                        //Valeur Seuil Bas
const unsigned int temps = 40;                                                                  //Temps d'un bit

//SETUP***********************************************************************************************************************************
void setup() 
{
  pinMode(ledPin, OUTPUT);
  pinMode(rfReceivePin, INPUT);
  Serial.begin(250000);
}

void loop() 
{
  //INITIALISATION*************************************************************************************************************************
  //Tant qu'on ne recois rien ... Ne rien faire
  data = analogRead(rfReceivePin);
  while (data <= 1000) 
  {
    data = analogRead(rfReceivePin);
  }
  Serial.println("Début");
  i = 0; //Reset i (Cas d'une seconde boucle)
  
  //Récupération des premiéres valeurs
  while (i < 40) 
  {
    data = analogRead(rfReceivePin);
    bit0[i] = data; //Bit0 = Signal Brut
    i++;
    delayMicroseconds(500);//wait 0.5 ms
  }

  //Remise à 0 de i si tableau plein de nouvelle valeur
  if (i == 40) 
  {
    i = 0;
    End = HIGH;
  }

  //BOUCLE PRINCIPALE*************************************************************************************************************************************
  while (End == HIGH) 
  {

    //Filtrage du signial et récupération continue des données
    bit1[j] = moyenne(bit0);
    j++;
    delayMicroseconds(500);
    data = analogRead(rfReceivePin);
    bit0[i] = data;
    i++;
 
    //Syncronisation en cours
    if (wait40 == LOW && wait10 == LOW) 
    {
      //Vérification si le signal filtré est au niveau HIGH
      if (bit1[j - 1] > upperThreshold) 
      {
        digitalWrite(ledPin, HIGH);   //Allumer la LED si Niveau HIGH
        donnee[l] = HIGH;             //donnee est le tableau ou l'on enrengistre les bit décodé
        wait40 = HIGH;                //Prochaine récupération dans 40ms
        l++;                          //Incrémentation de donnee
      }

      //Vérification si le signal filtré est au niveau LOW
      else 
      {
        if (bit1[j - 1] < lowerThreshold) 
        {
          digitalWrite(ledPin, LOW);   //Eteindre la LED si Niveau HIGH
          donnee[l] = LOW;
          wait40 = HIGH;
          l++;
        }
        else 
        {
          wait10 = HIGH; //Dans le cas ou ni le niveau HIGH ou LOW découvert on essaye de se resyncroniser avec le signal n attendant 10ms
        }
      }
      
      //Si syncro OK Récupération des données ET QUE C'EST LE MOMENT DE RECUPERE LES BITS!!!!
      if (syncro == HIGH) 
      {
        //recupérer 7*16 bits
        mot[v] = donnee[l - 1];
        v++;
        //Fin de la recupération de donnée
        if (v == 128) 
        {
          End = LOW;
          v = 0;
          j = 0;
          syncro = LOW;
        }
      }
    }
    
    //Les diférent tempo tout en récupérant les données!!
    if (wait10 == HIGH) 
    {
      if (w == 10) 
      {
        wait10 = LOW;
        w = 0;
      }
      w++;
    }
    if (wait40 == HIGH) 
    {
      if (w == 40) 
      {
        wait40 = LOW;
        w = 0;
      }
      w++;
    }
    
    //Reset i, j et l (Dans le cas ou on arrive au bout du tableau on réécrit par dessus les précédente valeur
    if (i == 40) 
    {
      i = 0;
    }
    if (j == 100) 
    {
      j = 0;
    }
    if (l == 50) 
    {
      l = 0;
    }
    
    //Repérage de la Syncro
    if (syncro == LOW) 
    {
      testsyncro = 0;
      for (int n = 0; n < 8; n++) 
      {
        if (donnee[l - 8 + n] == syncrotab[n]) 
        {
          testsyncro++; //On regarde si les 8 derniers bit sont égal a la syncro demandé
        }
        //Si Signal de Syncro est trouvé
        if (testsyncro == 8) 
        {
          syncro = HIGH;
          Serial.println("Syncro OK");
          /*
                    //afichage LCD
                    if(affichage==0)
                    {
                      lcd.init();
                      lcd.print("S");
                      lcd.setCursor(1,0);
                      lcd.print("y");
                      lcd.setCursor(2,0);
                      lcd.print("n");
                      lcd.setCursor(3,0);
                      lcd.print("c");
                      lcd.setCursor(4,0);
                      lcd.print("h");
                      lcd.setCursor(5,0);
                      lcd.print("r");
                      lcd.setCursor(6,0);
                      lcd.print("o");
                      lcd.setCursor(7,0);
                      lcd.print(" ");
                      lcd.setCursor(8,0);
                      lcd.print("O");
                      lcd.setCursor(9,0);
                      lcd.print("K");
                    }*/
        }
      }
    }
  }
  //FIN DE LA BOUCLE-ON DECHIFFRE LES DONNEES RECU******************************************************************************************************
  Serial.println("Le mot est donc :");
  cptlcd = 0, o = 0;

  for (int t = 0; t < 16; t++) 
  {
    //Récupération de chaque lettre du "mot"
    for (int d = 1; d < 8; d++)
    {
      lettre[d] = mot[(t * 8) + d];
    }
    charBit = bits(lettre); //Passage de binaire en charactére avec la fonction bits
    Serial.print(" ");
    Serial.print(charBit); //On affice la lettre recu
    Serial.println();
    motf[o] = charBit; //Mise de la lettre dans un tableau
    o++;
  }

  //affichage LCD
  lcd.init();
  for (int q = 0; q < 16; q++) 
  {
    if (affichage == 0)
    {
      lcd.setCursor(cptlcd, 0);
      lcd.print(motf[q]);
    }
    cptlcd++;
    affichage = 0;
  }
}
//FIN DU PROGRAMME********************************************************************************************************************************

//FONCTIONS***************************************************************************************************************************************
//Fonction de calcul de la moyenne du signal
double moyenne(int tab[40]) 
{
  double moy = 0;
  double som = 0;
  for (int k = 0; k < 40; k++) 
  {
    som = som + tab[k];
  }
  moy = som / 40;

  return moy;
}

//Transformation d'un octet(boolean) en lettre(char)
char bits(boolean let[8]) 
{
  char lettre;
  int bits[8];
  double somme = 0;
  
  //Transformation des HIGH et LOW en 1 et 0
  for (int d = 0; d < 8; d++) 
  {
    if (let[d] == HIGH) 
    {
      bits[d] = 1;
      Serial.print("1");
    }
    if (let[d] == LOW) 
    {
      bits[d] = 0;
      Serial.print("0");
    }
    //Calcul du chiffre décimal corespondant a la table ASCII
    float expo = 7 - d, pui = 2;
    somme = somme + (bits[d] * pow(pui, expo));
  }
  
  int sommeint = somme;
  Serial.print(" ");
  Serial.print(sommeint + 1);
  lettre = char(sommeint + 1);
  return lettre;
}

