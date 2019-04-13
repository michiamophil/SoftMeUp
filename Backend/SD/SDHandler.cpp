//#include "SDHandler.h"
//#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
//Sd2Card card;
//SdVolume volume;
//SdFile root;

//~ int vibrazione; // 0 NO - 1..N differenti tipi di vibrazione
//~ String songName = String(8); // NOTA non piu di 8 caratteri di nome e formato wave
//~ int volume;

//~ struct sveglia{	
	//~ char ora;
	//~ char min;
	//~ char canzone[8];
	//~ char vibrazione;
	//~ char volume;
//~ } sv;

using namespace std;

//~ String getDataFromFile(String fn){

	//~ String data = String(10);
	//~ File dt = SD.open("SETTINGS/"+fn);
	//~ while(dt.available()){
		//~ data += dt.read();
		//~ //Serial.write(data);
		//~ //Serial.print(data);
	//~ }
	//~ dt.close();
	//~ return data;
//~ }

//~ void writeDataToFile(String data, String fn){
	//~ File dt = SD.open("SETTINGS/"+fn, FILE_WRITE);
	//~ dt.print(data);
	//~ dt.close();	
//~ }

//~ void storeAlarmVariables(){
	
//~ }


//~ void refreshAllAllarmVariables(){
    //~ sv.ora = getDataFromFile("ORA.dat").toInt();
    //~ sv.min = getDataFromFile("MIN.dat").toInt();
    //~ vibrazione = getDataFromFile("VIBR.dat").toInt();
    //~ songName = getDataFromFile("SONGNAME.dat");
    //~ volume = getDataFromFile("VOLUME.dat").toInt();
//~ }

//~ void saveAllAlarmVariablesToSD(){
	//~ writeDataToFile((String)sv.ora,"ORA.dat");
	//~ writeDataToFile((String)sv.min,"MIN.dat");
	//~ writeDataToFile((String)vibrazione,"VIBR.dat");
	//~ writeDataToFile(songName,"SONGNAME.dat");
	//~ writeDataToFile((String)volume,"VOLUME.dat");
//~ }

int InitSD(int CS){
	
	if (!SD.begin(CS)) return 1;
	//if (!card.init(SPI_HALF_SPEED, CS)) return 2;
	//if (!volume.init(card)) return 3;
	
	if(!SD.exists("DATA")) SD.mkdir("DATA");
	if(!SD.exists("SETTINGS")) SD.mkdir("SETTINGS");
	

	//~ sv.ora = 8,
	//~ sv.min = 30;
	//~ songName = "HM";
	//~ volume = 2;
	//~ vibrazione = 3;	
	//~ saveAllAlarmVariablesToSD();
	//Serial.print(getDataFromFile("SONGNAME.dat"),BIN);
	//getDataFromFile("SONGNAME.dat");
	//~ refreshAllAllarmVariables();
	//~ Serial.println("");
	//~ Serial.print(songName);
	//~ Serial.println("");
	//~ songName = "HG";
	//~ Serial.print(songName);
	//~ Serial.println("");
	long int a;
	float b;
	char c;
	
	//a = 69;
	//EEPROM.write(0,a);
	
	a = EEPROM.read(0);
	Serial.println(a,DEC);
	Serial.println(sizeof(a));
	Serial.println(sizeof(b));
	Serial.println(sizeof(c));
	
	return 0;
}
