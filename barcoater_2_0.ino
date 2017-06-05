#include <Wire.h>


#define DIR_PIN 13
#define STEP_PIN 12
#define DEPART_PIN 8
#define RETOUR_PIN 9
#define VIT_PIN A1
#define COURSE_PIN A2
#define INTER_PIN 2

#define vitesse_retour 100      //vitesse retour  en mm/s
#define module 40               // moduleXnd de temps de la poulie motrice ( 
  


void setup() { 
  
  
  pinMode(DIR_PIN, OUTPUT); 
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DEPART_PIN, INPUT);
  pinMode(RETOUR_PIN, INPUT);
  digitalWrite(DEPART_PIN,HIGH);    // ajoute Pull-Up interne au pin.
  digitalWrite(RETOUR_PIN,HIGH);
  digitalWrite(INTER_PIN,HIGH);
  
 
   //

  Wire.begin();               // ouverture de l'I2C
  
  //ecriture des labels sur LCD
  
  Wire.beginTransmission(0); // adresse de l'afficheur (ici 0)
    Wire.write(0x0);           // initialisation ( 3 fois 0x0)
    Wire.write(0x0);
    Wire.write(0x0);
    Wire.write(0x1B);           // 2 codes pour effaccement de l'ecran.
    Wire.write(0x43);
  
    Wire.write(0x1B);  // 2 code position du curseur
    Wire.write(0x4C); 
    Wire.write(1);      // Position X
    Wire.write(0);      //Position Y
    Wire.write("vit mm/s");
    
    Wire.write(0x1B);  // 2 codes postion du curseur
    Wire.write(0x4C);
    Wire.write(1);     
    Wire.write(1);
    Wire.write("course mm");
    
  Wire.endTransmission();    // stop transmitting
  
    // initialisation position du chariot à zero+10mm
    int flag=1;                  // initialisation entrée interupteur
    digitalWrite (DIR_PIN,LOW); // direction retour home
    
   while (flag==1){           // boucle les pas moteur et arret si fin de course atteint
    digitalWrite(STEP_PIN, HIGH); 
      delayMicroseconds(200);
      digitalWrite(STEP_PIN, LOW); 
      delayMicroseconds(200); 
      flag = digitalRead (INTER_PIN);      // lecture des interupteur de fin de course

    }
    
    //  Avance de 10mm 
  
    
  int i=0;
  digitalWrite (DIR_PIN,HIGH);            // direction retour home
   while (i<=800){                         // boucle les pas moteur et arret si fin de course atteint
    digitalWrite(STEP_PIN, HIGH); 
      delayMicroseconds(500);
      digitalWrite(STEP_PIN, LOW); 
      delayMicroseconds(500); 
       i++;
       // lecture des interupteur de fin de course

    }
  
  
  
  
    } 


void loop(){

  
  //lecture de la vitesse sur le potentiomentre
  int val1 = analogRead(VIT_PIN);
  int vitesse_depot = map(val1, 0, 1023, 250, 10);  // mapping de la vitesse Min =10 mm/s  Max=250mm/S
  int val2 = analogRead (COURSE_PIN);
  int course = map(val2, 0, 1023, 250, 10);
  
  //conversion des donnée de lecture pour affichage dans le LCD
  
  
  String bouton1(vitesse_depot, DEC);   //converti la vitesse_depot en chaine de carractere
  String bouton2(course, DEC);
  char charBuf1[10];                     // cree un tableau avec les carracteres de string
  bouton1.toCharArray(charBuf1, 10) ;    // Convertie la variable String en tableau
  char charBuf2[10];
  bouton2.toCharArray(charBuf2, 10) ;
  
  //Affichage des données de lecture dans le LCD par le bus I2C
 
  Wire.beginTransmission(0);
      
    Wire.write(0x1B);  // 2 codes position du curseur
    Wire.write(0x4C); 
    Wire.write(0);      // Position X
    Wire.write(0);      //Position Y

for (int i=0; i < 3; i++){  // ecrit les 3 premiers carractere du tableau dans le LCD
    
    Wire.write(charBuf1[i]);
}
    Wire.write(0x1B);  // 2 codes position du curseur
    Wire.write(0x4C); 
    Wire.write(0);      // Position X
    Wire.write(1);      //Position Y
for (int i=0; i < 3; i++){     //ecrit les 3 premier carractere du tableau sur le LCD
    
  Wire.write(charBuf2[i]);
}   
  Wire.endTransmission();  // fin de transmission


 
 // lecture des bouton et action sur les moteur

  if (digitalRead(DEPART_PIN) == LOW) {      //lecteure bouton depart
    delay(100);
    rotate(course, vitesse_depot);
    }
  if (digitalRead(RETOUR_PIN) == LOW) {      //lecture bouton retour
    delay (100);
    rotate(-course, vitesse_retour);         // Signe negatif de la  vitesse pour le retour
    int i=0;
    digitalWrite(DIR_PIN,HIGH);              // petit retour en avant pour decoller l'inter de fin de course
    while (i < 200 ){           
    digitalWrite(STEP_PIN, HIGH); 
      delayMicroseconds(500);
      digitalWrite(STEP_PIN, LOW); 
      delayMicroseconds(500); 
       i++;
    }
  
}
 
 }

//Fonction d'envoie des impulsion au driver du moteur. 1 implusion=1 µstep du moteur.

  void rotate(float travel, float vitesse){ 
    //tourne à la vitesse designée en mm/s , negatif pour un mouvement inversé
   int flag= 1;

    int dir = (travel > 0)? HIGH:LOW;          //detection de la direction du mouvement 
    digitalWrite(DIR_PIN,dir);                 // Activation du PIN direction

    int steps = abs(travel/40*3200);           //calul du nombre de pas a effectuer
    float usDelay = (156/vitesse*module);      //calcul de la longueur d'implusion 300µs=3200 µstep/s
    int i=0;

    while (i < steps && flag==1){           // boucle les pas moteur et arret si fin de course atteint
    digitalWrite(STEP_PIN, HIGH); 
      delayMicroseconds(usDelay);
      digitalWrite(STEP_PIN, LOW); 
      delayMicroseconds(usDelay); 
         i++;
      flag = digitalRead (INTER_PIN);      // lecture des interupteur de fin de course

    }

    }
    
    
   
 

