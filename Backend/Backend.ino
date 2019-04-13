//#include "SD/SDHandler.cpp"
//#include "settings.cpp"
//#include "TMRpcm.h"
//#include <EEPROM.h>
#include "SD/variables.cpp"
//TMRpcm test;

void setup() {
  //test.speakerPin=9;  
  // Inizializzo la comunicazione seriale
  Serial.begin(9600); while (!Serial) {;}

  //Serial.println("Inizializzo SD...");

  initVariables(); //Inizializzo tutte le variabili globali
  // INFO
  //Serial.print("Exit status: ");
  //Serial.print(InitSD(4)); // Exit status 0 tutto a posto.
  //InitEEPROM();
  //test.setVolume();
  //test.play("HM.wav");
  //getDataFromFile();
  //Serial.print(sizeof(sv));
  globalVariablesDump();
}

void loop() {
}
