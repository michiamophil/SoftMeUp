#define ALARMCODE 'A'
#define ALARMLENGTH 10 // 12 caratteri, SONO compresi il bit iniziale e finale
#define SETTINGSCODE 'B'
#define SETTINGSLENGTH 1
#define CLOCKCODE 'C'
#define CLOCKLENGTH 6

#define DUMPVARIABLES 'd' // FOR DEBUG
#define PRINTCURRENTTIME 'e' // FOR DEBUG
#define RESET 'r' // FOR DEBUG

#define RECEIVINGONEDIGITDELAY 50

#include <SoftwareSerial.h>
#include <Streaming.h>

extern void lightsOn();
extern void lightsOff();

bool bluetoothIsActive = false;

SoftwareSerial bluetooth = SoftwareSerial(BTTXPIN,BTRXPIN);
// attenzion: cavo rx del bluetooth necessita di voltage divider, meglio non collegarlo

void(* resetFunc) (void) = 0;


void bluetoothChangeState(){
	if (bluetoothIsActive){
		// SPENGO IL BLUETOOTH
			//if(bluetoothISReceiving){
			//	Serial << "ATTENZIONE: Il bluetooth sta trasferendo dati, impossibile spegnerlo.\n";
			//} else {
		Serial << "Spengo il bluetooth.\n\n";
		digitalWrite(BTVCCPIN, LOW);
		bluetoothIsActive = false;
			//}
	} else {
		// INIZIALIZZO E ACCENDO IL BLUETOOTH
		Serial << "Accendo e inizializzo il bluetooth.\n\n";
		bluetooth = SoftwareSerial(BTTXPIN,BTRXPIN);
		digitalWrite(BTVCCPIN, HIGH);
		delay(1000);
		bluetooth.begin(BLUETOOTHBAUD);
		delay(500);
		
		if(bluetooth.available()){bluetooth.read();} // Libero il buffer dal carattere 0 che scrive ad ogni sprecedente pegnimento
		
		bluetoothIsActive = true;
	}
}



void bluetoothReceivedDataAlarmHandler(char *buffer){
	
	// STACK condiviso con l'app, vedi messaggio telegram con Sergio
	sv.thisAlarmIsActive = (buffer[0] == '1' ? true : false);
	sv.ora = (buffer[1] - '0') * 10 + (buffer[2] - '0');
	sv.min = (buffer[3] - '0') * 10 + (buffer[4] - '0');
	sv.vibrazione = buffer[5] - '0';
	sv.luci = buffer[6] - '0';
	sv.snooseTimer = (buffer[7] - '0') * 10 + (buffer[8] - '0');
	sv.snoozeCount = buffer[9] - '0';
	
	push(ALARM);
	setRTCInterruptAlarm(sv.ora, sv.min);
	
	Serial << "Sveglia settata con successo\n";
	//globalVariablesDump();
}

void bluetoothReceivedDataSettingsHandler(char *buffer){
	settingsButtonHardStateLongPressOn = (buffer[0] == '1' ? true : false);
	push(GSETTINGS);
	Serial << "Impostazione settata con successo\n";
}

void bluetoothReceivedDataClockHandler(char *buffer){
	char hh, mm, ss;
	hh = (buffer[0] - '0') * 10 + (buffer[1] - '0');
	mm = (buffer[2] - '0') * 10 + (buffer[3] - '0');
	ss = (buffer[4] - '0') * 10 + (buffer[5] - '0');
	setRTCTime(1969,7,20,hh,mm,ss); // 20 luglio 1969 - Ehi tu che stai guardando questo pezzo di codice, sai che data è?
	Serial << "Nuovo orario ricevuto dall'applicazione e settato con successo\n";
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
				case DUMPVARIABLES:
					//globalVariablesDump();
					return;
					break;
				case PRINTCURRENTTIME :
					printTime(RTC.get());
					return;
					break;
				case RESET:
					digitalWrite(BTVCCPIN, LOW);
					resetFunc();
					break;
				default:
					Serial << "ERRORE (BT): codice iniziale pacchetto errato.. ABORTING!\n\n";
					return;	// Inizia la ricezione del pacchetto solamente se il code iniziale é corretto
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
			Serial << "EVENT (BT): pacchetto ricevuto!\n";
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
				default:
					Serial << "ERRORE (BT): ho ricevuto una stringa errata!\n";
					//Serial << "Ovviamente vorrai sapere che comunicazione era... ti do un indizio: " << communicationLength << "\n";
					lightsOn(); delay(2000); lightsOff(); // USER FEEDBACK				
			}

		}
	}
}
