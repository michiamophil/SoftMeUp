// 13 Aprile 2019
// Filippo Bordon
//
// Gestione e salvataggio variabili globali programma
// 
// DA INTEGRARE NEL MAIN:
//
// initVariables();
//
//
// il programma seguente salva e ripristina le variabili:
// - password
// - sveglia
// 
//
// Per effettuare un print delle variabili di sistema richiamare la funzione globalVariablesDump()

#include <EEPROM.h>

#define DEFAULTTHRESHOLD 30

//=DO NOT MODIFY - EEPROM STACK====
#define SIGNADDR 2
#define ALARMADDR 10
#define PWDADDR 24
#define GSETTINGSADDR 26
#define DEBUGADDR 27
#define THRESHOLD1ADDR 28
#define THRESHOLD2ADDR 30
#define BLUETOOTHADDR 32

#define PWD 0
#define ALARM 1
#define SIGNATURE 2
#define GSETTINGS 3
#define THRESHOLD1 5
#define THRESHOLD2 6

#define SIGNVAL 175
//=================================

extern void BPrint(String msg);
extern void BPrintNoNl(String);
extern void BPrintDec(int n);

typedef struct sveglia{
	bool thisAlarmIsActive;
	char ora;
	char min;
	char vibrazione;
	char tipo_vibrazione;
	char luci;
	char tipo_luci;
	char snoozeCount; // In soft mode: numero di volte che si puo' premere lo snooze prima di entrare in Hard Mode wake-up. (se vale $UNLIMITEDSNOOZE, non entra mai in hard mode)
	char snooseTimer; // Tempo di attesa per la successiva sveglia, es. 5 min
} t_sveglia;

t_sveglia sv; // = { 0, 0, 0, "FAILSAFE", 2, 3, 1};


unsigned int pwd; // Password usata per connettersi col bluetooth al device.. Default: 1234

bool settingsButtonHardStateLongPressOn;
bool switchAutoBluetoothOff;

bool debugIsActive;

unsigned int threshold1, threshold2;

// Salva
void push(char choice){
	switch(choice){
		case PWD : {
			EEPROM.put(PWDADDR, pwd);
			break;
		}
		case ALARM : {
			EEPROM.put(ALARMADDR, sv);
			break;
		}
		case SIGNATURE : {
			unsigned char sign = SIGNVAL; 
			EEPROM.put(SIGNADDR, sign); // non é possibile modificarla dall'esterno
			break;
		}
		case GSETTINGS : {
			char s = (settingsButtonHardStateLongPressOn ? 1 : 0);
			EEPROM.put(GSETTINGSADDR, s);
			char b = (switchAutoBluetoothOff ? 1 : 0);
			EEPROM.put(BLUETOOTHADDR, b);
			char d = (debugIsActive ? 1 : 0);
			EEPROM.put(DEBUGADDR, d);
			break;
		}
		case THRESHOLD1 : {
			EEPROM.put(THRESHOLD1ADDR, threshold1);
			break;
		}
		case THRESHOLD2 : {
			EEPROM.put(THRESHOLD2ADDR, threshold2);
			break;
		}
	}
	
}

// Ripristina
void pop(char choice){
	switch(choice){
		case PWD : {
			EEPROM.get(PWDADDR, pwd);
			break;
		}
		case ALARM : {
			EEPROM.get(ALARMADDR, sv);
			break;
		}
		case GSETTINGS : {
			char s;
			EEPROM.get(GSETTINGSADDR, s);
			settingsButtonHardStateLongPressOn = (s == 1 ? true : false);
			char b;
			EEPROM.get(BLUETOOTHADDR, b);
			switchAutoBluetoothOff = (b == 1 ? true : false);
			char d;
			EEPROM.get(DEBUGADDR, d);
			debugIsActive = (d == 1 ? true : false);
			break;
		}
		case THRESHOLD1 : {
			EEPROM.get(THRESHOLD1ADDR, threshold1);
			break;
		}
		case THRESHOLD2 : {
			EEPROM.get(THRESHOLD2ADDR, threshold2);
			break;
		}
	}
}

void initVariables(){
	
	// check EEPROM
	unsigned char sign;
	EEPROM.get(SIGNADDR, sign);
	if(sign != SIGNVAL){ // Se non é inizializzata la inizializzo
		//BPrint("N12");
		sv = t_sveglia{0, 8, 00, 5, 3, 9, 3, 15, 3};
		pwd = 1234;
		threshold1 = DEFAULTTHRESHOLD;
		threshold2 = DEFAULTTHRESHOLD;
		debugIsActive = false;
		settingsButtonHardStateLongPressOn = false;
		switchAutoBluetoothOff = false;
		
		push(ALARM); push(PWD); push(SIGNATURE); push(GSETTINGS); push(THRESHOLD1); push(THRESHOLD2);
	} else { // se é inizializzata ripristino lo stato precedente delle variabili
		//BPrint("N13");
		pop(ALARM);
		pop(PWD);
		pop(GSETTINGS);
		pop(THRESHOLD1);
		pop(THRESHOLD2);
	}
}


void globalVariablesDump(){
	//Serial.println("Dumpo le variabili globali del programma... Enjoy your debug!");
	//String c = ((int)sv.thisAlarmIsActive == 1) ? "A" : "D";
	BPrintNoNl(((int)sv.thisAlarmIsActive == 1) ? "A " : "D "); BPrintDec((int)sv.ora); BPrintNoNl(":"); BPrintDec((int)sv.min); BPrint(" ");  BPrintDec((int)sv.snoozeCount); BPrintNoNl(" - "); BPrintDec((int)sv.snooseTimer); BPrint(" ");
	BPrintNoNl("\nD: "); BPrint((debugIsActive == true) ? "Y" : "N");
	BPrintNoNl("\nB: "); BPrint((switchAutoBluetoothOff == true) ? "Y" : "N");
}

