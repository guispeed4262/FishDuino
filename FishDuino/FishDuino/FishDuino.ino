

// Auteur du Programme : X. HINAULT - Tous droits réservés
// Programme écrit le : 12/2011.

// ------- Licence du code de ce programme -----
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License,
//  or any later version.
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <LiquidCrystal.h> // Ajout librairie liquidCristal
#include <SD.h> // crée automatiquement un objet racine SD représentant la carte mémoire SD
#include <dht11.h>
dht11 DHT11;
#define DHT11PIN 8 // on definit la pin digital 8



File myFile; // objet file 
File myDir; // objet file 

char dirName[]="/monrep"; // tableau de caracteres pour le nom du répertoire

// nom du fichier court (8 caractères maxi . 3 caractères maxi) (dit format DOS 8.3)
char fileName[] = "test.txt"; // tableau de caractères pour le nom du fichier

char pathFile[] = "/monrep/test.txt"; // tableau de caractères pour le nom du fichier

int test; //variable mémorisation résultat renvoyé par fonctions


///////////////// LCD ///////////////////////
// --- constantes des broches ---

const int RS=2; //declaration constante de broche 
const int E=3; //declaration constante de broche 

const int D4=4; //declaration constante de broche 
const int D5=5; //declaration constante de broche 
const int D6=6; //declaration constante de broche 
const int D7=7; //declaration constante de broche 
//////////////////// FIN LCD //////////////////////
///////////////// relais //////////////////////
const int RELAIS = 9;
/////////// Fin Relais /////////////////////


// --- Déclaration des variables globales ---

////////////////////////// LCD ///////////////////
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);// initialisation LCD en mode 4 bits 
// Caractere speciaux cree

byte degree[8] = {
  B10000010,
  B10000101,
  B10000010,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
};
byte poisson1[8] = {
  B10000001,
  B10000011,
  B10010111,
  B10011111,
  B10011111,
  B10010111,
  B10000011,
  B10000001,
};
byte poisson2[8] = {
  B10011000,
  B10011100,
  B10010010,
  B10010011,
  B10011111,
  B10011110,
  B10011100,
  B10011000,
};
///////////////////// FIN LCD /////////////////////

void setup()
{  
  Serial.begin(115200);
  //initLCD1();
  initLCD();
  initSdCard();
  pinMode(RELAIS,OUTPUT);
  digitalWrite(RELAIS,LOW); 
} // fin setup()  

int heure[] = {0,58,23};
int ecran=1;
void loop(){
  initLCD1();
  //delay(2000);
  //digitalWrite(RELAIS,HIGH); 
  //initLCD2();
  setHeure(heure);
  checkTemp(ecran);
  
  
  
  
  delay(1000); // pause de 2 secondes
  //digitalWrite(RELAIS,LOW); 
}

void initLCD(){
  lcd.begin(20,4);
  lcd.createChar(0, degree);
  lcd.createChar(1,poisson1);
  lcd.createChar(2,poisson2);
}
void initLCD1(){
  //lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print((char)1);
  lcd.print((char)2);
  lcd.print((char)1);
  lcd.print((char)2);
  lcd.print(" Fishroom ");
  lcd.print((char)1);
  lcd.print((char)2);
  lcd.print((char)1);
  lcd.print((char)2);
  lcd.setCursor(0, 1);
  lcd.print("Hum:");
  lcd.print("00%");
  lcd.setCursor(10, 1);
  lcd.print("00:00:00");
  lcd.setCursor(0, 2);
  lcd.print("Air:");
  lcd.print("00");
  lcd.setCursor(6, 2);
  lcd.print((char)0);
  lcd.print("C");
  lcd.setCursor(10, 2);
  lcd.print("00/00/2012");
}
void initLCD2(){
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print((char)1);
  lcd.print((char)2);
  lcd.print(" Information ");
  lcd.print((char)1);
  lcd.print((char)2);
  lcd.setCursor(0, 1);
  lcd.print("  Ext ");
  lcd.setCursor(13, 1);
  lcd.print("Cave");
  lcd.setCursor(0, 2);
  lcd.print("Hum:");
  lcd.print("00%");
  lcd.setCursor(0, 3);
  lcd.print("Air:");
  lcd.print("00");
  lcd.setCursor(6, 3);
  lcd.print((char)0);
  lcd.print("C");
  lcd.setCursor(11, 2);
  lcd.print("Air:");
  lcd.print("00");
  lcd.print((char)0);
  lcd.print("C");
}

void initSdCard(){
  //----- initialisation de la carte SD ----- 
  Serial.println("Initialisation de la SD card...");
  pinMode(10, OUTPUT); // laisser la broche SS en sortie - obligatoire avec librairie SD
  test=SD.begin(4); // initialisation de la carte SD avec broche 4 en tant que CS - renvoie true/false
  if (test!=true) { // si initialisation n'est pas réussie
    Serial.println("Echec initialisation!"); // message port Série
  }
  else { // si nitialisation réussie
    Serial.println("Initialisation reussie !"); // message port Série
  } // fin si SD.begin
  //----- test de l'existence d'un répertoire
  test=SD.exists(pathFile); // test si le répertoire existe - renvoie true/false
  Serial.print ("Test="); Serial.println(test); 
  if (test!=true) { // si le répertoire n'existe pas
    Serial.println ("Le fichier n'existe pas !");
  }else { // si le répertoire existe 
    Serial.println ("Le fichier existe !"); 
    //SD.remove(pathFile); // efface le fichier   
    //Serial.println ("Fichier efface !");
    //----- ouverture du fichier ---
    myFile=SD.open(pathFile,FILE_WRITE); // ouvre le fichier en écriture et crée fichier si existe pas
    //Serial.print ("Test="); Serial.println(test); 
    if (myFile!=true) { // si échec ouverture
      Serial.println ("Echec ouverture fichier !"); 
    }else { // si ouverture réussie
      Serial.println ("Ouverture fichier reussie !"); 
      myFile.seek(myFile.size()); // positionnement à la fin du fichier
      Serial.println ("Positionnement a la fin du fichier!"); 
      myFile.println(); // ajoute un saut de ligne 
      myFile.println("------ Debut nouvel enregistrement -------"); // ajoute une ligne
      myFile.println(millis()); // ajoute la ligne au fichier - ici enregistre la valeur de millis
      /*-- affiche une nouvelle série de 30 valeur
      for (int i=0; i<30; i++) {
        myFile.println(i); // ajoute la ligne au fichier
      }
      */
      myFile.close(); // fermeture du fichier
      Serial.println ("Fermeture fichier effectuee!");
      test=SD.exists(pathFile); // test si le fichier existe - renvoie true/false
      Serial.print ("Test="); Serial.println(test); 
      if (test!=true)  { // si fichier existe pas 
         Serial.println ("Le fichier existe pas !");
      }
       else { // si fichier existe
         Serial.println ("Le fichier existe !");
         //--- ouverture du fichier en lecture --- 
         myFile=SD.open(pathFile,FILE_READ); // ouvre le fichier en lecture 
         Serial.println ("Ouverture du fichier en lecture");
         for (int i=0; i<myFile.size(); i++ ) {
           myFile.seek(i); // se positionne à la position i
           Serial.print(char(myFile.peek())); // affiche le caractère courant
         }
       myFile.close(); // fermeture du fichier
       Serial.println ("Fermeture fichier effectuee!");
       } // fin else si fichier existe
     } // fin else si ouverture répertoire réussie
 }// fin else si répertoire
}

void checkTemp(int lcd){
  int chk = DHT11.read(DHT11PIN);
  Serial.print("Read sensor: ");
  switch (chk)
  {
    case 0: Serial.println("OK"); break;
    case -1: Serial.println("Checksum error"); break;
    case -2: Serial.println("Time out error"); break;
    default: Serial.println("Unknown error"); break;
  }
  
  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);
 
  Serial.print("Temperature (oC): ");
  Serial.println((float)DHT11.temperature, 2);
  if (lcd==1){
    setHum(DHT11.humidity);
    setTemp(DHT11.temperature);
  }

  Serial.print("Dew PointFast (oC): ");
  Serial.println(dewPointFast(DHT11.temperature, DHT11.humidity));




}
// delta max = 0.6544 wrt dewPoint() Point de rosée
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
	double a = 17.271;
	double b = 237.7;
	double temp = (a * celsius) / (b + celsius) + log(humidity/100);
	double Td = (b * temp) / (a - temp);
	return Td;
}

void setTemp(int temp){
  lcd.setCursor(4, 2);
  lcd.print(temp);
}
void setHum(int hum){
  lcd.setCursor(4, 1);
  lcd.print(hum);
}
int setHeure(int *heure){
  heure[0]++;
  if (heure[0]==60){
   heure[0]=0;
   heure[1]++;
  }
 if (heure[1]==60){
  heure[1]=0;
  heure[2]++;
 }
  if(heure[2]==24){
  heure[2]=0;
  heure[1]=0;
  heure[0]=0;  
  }    
  lcd.setCursor(10, 1);
  if (heure[2]<10){
    lcd.print("0");
  }
  lcd.print(heure[2]);
  lcd.print(":");
  if (heure[1]<10){
    lcd.print("0");
  }
  lcd.print(heure[1]);
  lcd.print(":");
  if (heure[0]<10){
    lcd.print("0");
  }
  lcd.print(heure[0]);
}
void addHeure(){
  lcd.setCursor(12, 1);
  lcd.print("00:00:00");
}
void setDate(){


}



