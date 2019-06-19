#define ALARMCODE 'A'
#define ALARMLENGTH 13 // 15 caratteri, SONO compresi il bit iniziale e finale
#define SETTINGSCODE 'B'
#define SETTINGSLENGTH 3
#define CLOCKCODE 'C'
#define CLOCKLENGTH 6

#define THRESHOLDCODE 'T'
#define THRESHOLDLENGTH 4

#define TESTLUCICODE 'L'
#define TESTLUCILENGTH 2
#define TESTMOTORICODE 'V'
#define TESTMOTORILENGTH 2

#define DUMPFORCES 'f' // FOR DEBUG
#define DUMPVARIABLES 'd' // FOR DEBUG
#define PRINTCURRENTTIME 't' // FOR DEBUG
#define RESET 'r' // FOR DEBUG

#define RECEIVINGONEDIGITDELAY 50

#include <SoftwareSerial.h>
//#include <Streaming.h>

// LIGHTS.H
extern void lightsOn();
extern void lightsOff();
extern void lightsChangeState();

// MOTORS.H
extern void motorsOn();
extern void motorsOff();
extern void motorsChangeState();

extern bool debugIsActive;
extern unsigned int threshold1, threshold2;
void printTime(time_t t);

bool bluetoothIsActive;


SoftwareSerial bluetooth = SoftwareSerial(BTTXPIN,BTRXPIN);
// attenzion: cavo rx del bluetooth necessita di voltage divider, meglio non collegarlo

void(* resetFunc) (void) = 0;


void bluetoothChangeState(){
	if (bluetoothIsActive){
		// SPENGO IL BLUETOOTH
			//if(bluetoothISReceiving){
			//	Serial << "ATTENZIONE: Il bluetooth sta trasferendo dati, impossibile spegnerlo.\n";
			//} else {
		BPrint("BT OFF\n");
		delay(500);
		digitalWrite(BTSTATUSVCCPIN, LOW);
		digitalWrite(BTVCCPIN, LOW);
		bluetoothIsActive = false;
			//}
	} else {
		// INIZIALIZZO E ACCENDO IL BLUETOOTH
		//Serial.println("BT ON\n");
		bluetooth = SoftwareSerial(BTTXPIN,BTRXPIN);
		digitalWrite(BTSTATUSVCCPIN, HIGH);
		digitalWrite(BTVCCPIN, HIGH);
		delay(1000);
		bluetooth.begin(BLUETOOTHBAUD);
		delay(500);
		
		if(bluetooth.available()){bluetooth.read();} // Libero il buffer dal carattere 0 che scrive ad ogni sprecedente pegnimento
		
		bluetoothIsActive = true;
	}
}

// Blink 3 times
void BTstatusLedBlink(){
	unsigned int d = 150;
	digitalWrite(BTSTATUSVCCPIN, LOW); delay(d); digitalWrite(BTSTATUSVCCPIN, HIGH); delay(d);
	digitalWrite(BTSTATUSVCCPIN, LOW); delay(d); digitalWrite(BTSTATUSVCCPIN, HIGH); delay(d);
	digitalWrite(BTSTATUSVCCPIN, LOW); delay(d); digitalWrite(BTSTATUSVCCPIN, HIGH); delay(d);
}



void bluetoothReceivedDataAlarmHandler(char *buffer){
	
	// STACK condiviso con l'app su smartphone
	sv.thisAlarmIsActive = (buffer[0] == '1' ? true : false);
	sv.ora = (buffer[1] - '0') * 10 + (buffer[2] - '0');
	sv.min = (buffer[3] - '0') * 10 + (buffer[4] - '0');
	sv.vibrazione = buffer[5] - '0';
	sv.tipo_vibrazione = buffer[6] - '0';
	sv.luci = buffer[7] - '0';
	sv.tipo_luci = buffer[8] - '0';
	sv.snooseTimer = (buffer[9] - '0') * 10 + (buffer[10] - '0');
	sv.snoozeCount = buffer[11] - '0' * 10 + (buffer[12] - '0');;
	
	push(ALARM);
	setRTCInterruptAlarm(sv.ora, sv.min, 0);
	
	BTstatusLedBlink();
	BPrint("Ok\n");
	//Spengo automaticamebte il bluetooth dopo aver settato la sveglia
	if(switchAutoBluetoothOff == true) { bluetoothChangeState(); }
}

void bluetoothReceivedDataSettingsHandler(char *buffer){
	settingsButtonHardStateLongPressOn = (buffer[0] == '1' ? true : false);
	switchAutoBluetoothOff = (buffer[1] == '1' ? true : false);
	debugIsActive = (buffer[2] == '1' ? true : false);
	push(GSETTINGS);
	
	BTstatusLedBlink();
	BPrint("Ok");
}

void bluetoothReceivedDataClockHandler(char *buffer){
	char hh, mm, ss;
	hh = (buffer[0] - '0') * 10 + (buffer[1] - '0');
	mm = (buffer[2] - '0') * 10 + (buffer[3] - '0');
	ss = (buffer[4] - '0') * 10 + (buffer[5] - '0');
	setRTCTime(1969,7,20,hh,mm,ss); // 20 luglio 1969 - Ehi tu che stai guardando questo pezzo di codice, sai che data è?
	
	BTstatusLedBlink();
	BPrint("Ok"); 
}

void bluetoothReceivedDataThresholdHandler(char *buffer){
	threshold1 = (buffer[0] - '0') * 10 + (buffer[1] - '0');
	threshold2 = (buffer[2] - '0') * 10 + (buffer[3] - '0');
	
	push(THRESHOLD1); push(THRESHOLD2);
	BTstatusLedBlink();
	BPrint("Ok"); 
}

void bluetoothReceivedDataTestVibrazione(char *buffer){
	sv.vibrazione = (buffer[0] - '0');
	sv.tipo_vibrazione = (buffer[1] - '0');
	BPrint("Ok"); 
	
	motorsOn();
	int i = 0;
	for(i; i<70; i++) { //Loop di 3 secondi
		motorsChangeState();
		delay(40);
	}
	motorsOff();	
}

void bluetoothReceivedDataTestLuci(char *buffer){
	sv.luci = (buffer[0] - '0');
	sv.tipo_luci = (buffer[1] - '0');
	BPrint("Ok"); 
	
	lightsOn();
	int i = 0;
	for(i; i<70; i++) {	//Loop di 3 secondi
		lightsChangeState();
		delay(40);
	}
	lightsOff();	
}

void bluetoothForcePrintLoop(){
	char i = 0;
	for( i; i<50; i++ ){
		BPrintNoNl("Forza: "); BPrintDec(forceCheck(FORCE1PIN)); BPrintNoNl(" - "); BPrintDec(forceCheck(FORCE2PIN)); BPrint("");
		delay(50);
	}
	BPrint("END");
}



void bluetoothCheck(){
	
	if(bluetoothIsActive){
		if (bluetooth.available()){
			char c, communicationLength;
			

			// CONTROLLO CODICE INIZIALE
			c = bluetooth.read();
			//Serial << c;
			switch(c){
				case ALARMCODE :
					communicationLength = ALARMLENGTH;
					break;
				case SETTINGSCODE :
					communicationLength = SETTINGSLENGTH;
					break;
				case CLOCKCODE :
					communicationLength = CLOCKLENGTH;
					break;
				case THRESHOLDCODE :
					communicationLength = THRESHOLDLENGTH;
					break;
				case TESTLUCICODE : {
					communicationLength = TESTLUCILENGTH;
					break;
				}
				case TESTMOTORICODE : {
					communicationLength = TESTMOTORILENGTH;
					break;
				}
				
				// DEBUG
				case DUMPVARIABLES: {
					globalVariablesDump();
					return;
					break;
				}
				case DUMPFORCES: {
					// ARDUINO WILL BE FREEZED FOR SOME SECONDS
					bluetoothForcePrintLoop();
					return;
					break;
				}
				case PRINTCURRENTTIME : {
					printTime(RTC.get());
					return;
					break;
				}
				case RESET: {
					// TURN OFF BLUETOOTH
					digitalWrite(BTSTATUSVCCPIN, LOW);
					digitalWrite(BTVCCPIN, LOW);
					bluetoothIsActive = false;
					resetFunc();
					break;
				}
				default: {
					BPrint("Reject");
					return;	// Inizia la ricezione del pacchetto solamente se il code iniziale é corretto
				}
			}
			
			// RICEZIONE DEL CORE DEL MESSAGGIO
			char i = 0;
			char buffer[communicationLength]; // Senza il carattere iniziale e finale di controllo
			for ( i; i < (communicationLength); i++ ){ // - 1 perché verificheró poi che l'ultimo carattere sia il code corretto
				delay(RECEIVINGONEDIGITDELAY);
				buffer[i] = bluetooth.read();
			}
			
			
			// RICEZIONE CODICE FINALE, CHECK E INSTRADAMENTO DEL BUFFER
			delay(RECEIVINGONEDIGITDELAY);
			c = bluetooth.read();
			//Serial << c << "\n";
			switch(c){
				case ALARMCODE :
					bluetoothReceivedDataAlarmHandler(&buffer[0]);
					break;
				case SETTINGSCODE :
					bluetoothReceivedDataSettingsHandler(&buffer[0]);
					break;
				case CLOCKCODE :
					bluetoothReceivedDataClockHandler(&buffer[0]);
					break;	
				case THRESHOLDCODE :
					bluetoothReceivedDataThresholdHandler(&buffer[0]);
					break;
				case TESTLUCICODE :
					bluetoothReceivedDataTestLuci(&buffer[0]);
					break;
				case TESTMOTORICODE :
					bluetoothReceivedDataTestVibrazione(&buffer[0]);
					break;
				default: {
					BPrint("Reject");
					//Serial << "Ovviamente vorrai sapere che comunicazione era... ti do un indizio: " << communicationLength << "\n";
					//lightsOn(); delay(2000); lightsOff(); // USER FEEDBACK	
					break;
				}		
			}

		}
	}
}

// Send via bluetooth the debug messages
//  if the global variable debugIsActive is true
void BPrint(String msg){
	if(debugIsActive == true) {
		if(bluetoothIsActive){
			bluetooth.print(msg);
			bluetooth.print("\n");
		
		}
	}
}

void BPrintDec(int n){
	if(debugIsActive == true) {
		if(bluetoothIsActive){
			bluetooth.print(" ");
			bluetooth.print(n, DEC);
			bluetooth.print(" ");
		}
	}
}


void printTime(time_t t) {
	//if(debugIsActive == true) {
		if(bluetoothIsActive){
			bluetooth.print("ORA: ");
			bluetooth.print(((hour(t)<10) ? "0" : "")); bluetooth.print((int)hour(t), DEC); bluetooth.print(":");
			bluetooth.print(((minute(t)<10) ? "0" : "")); bluetooth.print((int)minute(t), DEC); bluetooth.print(":");
			bluetooth.print(((second(t)<10) ? "0" : "")); bluetooth.print((int)second(t), DEC); bluetooth.print("\n");
		}
	//}

}
void BPrintNoNl(String msg){
	if(debugIsActive == true) {
		if(bluetoothIsActive){
			bluetooth.print(msg);
		}
	}
}

