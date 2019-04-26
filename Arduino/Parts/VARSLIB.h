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

//=DO NOT MODIFY - EEPROM STACK====
#define SIGNADDR 2
#define ALARMADDR 10
#define PWDADDR 20
#define GSETTINGSADDR 22

#define PWD 0
#define ALARM 1
#define SIGNATURE 2
#define GSETTINGS 3

#define SIGNVAL 175
//=================================


typedef struct sveglia{
	bool thisAlarmIsActive;
	char ora;
	char min;
	char vibrazione;
	char luci;
	char snoozeCount; // In soft mode: numero di volte che si puo' premere lo snooze prima di entrare in Hard Mode wake-up. (se vale $UNLIMITEDSNOOZE, non entra mai in hard mode)
	char snooseTimer; // Tempo di attesa per la successiva sveglia, es. 5 min
} t_sveglia;

t_sveglia sv; // = { 0, 0, 0, "FAILSAFE", 2, 3, 1};


unsigned int pwd; // Password usata per connettersi col bluetooth al device.. Default: 1234
//unsigned int sign; // Serve per capire se il device ha la eeprom inizializzata o no

bool settingsButtonHardStateLongPressOn;


// Salva
void push(char choice){
	switch(choice){
		case PWD :
			EEPROM.put(PWDADDR, pwd);
			break;
		case ALARM :
			EEPROM.put(ALARMADDR, sv);
			break;
		case SIGNATURE : {
			unsigned char sign = SIGNVAL; 
			EEPROM.put(SIGNADDR, sign); // non é possibile modificarla dall'esterno
			break;
		}
		case GSETTINGS : {
			char s = (settingsButtonHardStateLongPressOn ? 1 : 0);
			EEPROM.put(GSETTINGSADDR, s);
			break;
		}
	}
	
}

// Ripristina
void pop(char choice){
	switch(choice){
		case PWD :
			EEPROM.get(PWDADDR, pwd);
			break;
		case ALARM :
			EEPROM.get(ALARMADDR, sv);
			break;
		case GSETTINGS : {
			char s;
			EEPROM.get(GSETTINGSADDR, s);
			settingsButtonHardStateLongPressOn = (s == 1 ? true : false);
			break;
		}
	}
}

void initVariables(){
	
	// check EEPROM
	unsigned char sign;
	EEPROM.get(SIGNADDR, sign);
	if(sign != SIGNVAL){ // Se non é inizializzata la inizializzo
		Serial.println("INITIALIZE System Variables with default values...");
		sv = t_sveglia{0, 8, 00, 5, 9, 15, 3};
		pwd = 1234;
		push(ALARM); push(PWD); push(SIGNATURE);
	} else { // se é inizializzata ripristino lo stato precedente delle variabili
		Serial.println("RESUME System Variables from EEPROM to RAM...");
		pop(ALARM);
		pop(PWD);
		pop(GSETTINGS);
	}
}


void globalVariablesDump(){
	//Serial.println("Dumpo le variabili globali del programma... Enjoy your debug!");
	Serial.println("===========Sveglia===========");
	Serial.print("Attiva: "); Serial.println((sv.thisAlarmIsActive) ? "SI" : "NO");
	Serial.print("Ora: "); Serial.print(sv.ora,DEC); Serial.print(":"); Serial.println(sv.min,DEC);
	Serial.print("Intensità vibrazione: "); Serial.println(sv.vibrazione, DEC);
	Serial.print("Intensità luci: "); Serial.println(sv.luci, DEC);
	Serial.println("");
	//~ Serial.println("===========Password==========");
	//~ Serial.print("Pwd: ");Serial.println(pwd);
	//~ Serial.println("");
	Serial.println("=======Settings Globali======");
	Serial.print("Pressione prolungata bottone spegne la sveglia in Hard Mode: "); Serial.println((settingsButtonHardStateLongPressOn) ? "SI" : "NO");
	Serial.println("");
}
