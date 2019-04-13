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

//=DO NOT MODIFY====
#define SIGNADDR 2
#define ALARMADDR 10
#define PWDADDR 24

#define PWD 0
#define ALARM 1
#define SIGNATURE 2

#define SIGNVAL 17569
//==================

typedef struct sveglia{
	char active; // 1 YES - 0 NO
	char ora;
	char min;
	char canzone[9];
	char vibrazione;
	char volume;
} t_sveglia;

t_sveglia sv;// = { 0, 0, 0, "FAILSAFE", 2, 3};


unsigned int pwd; // Password usata per connettersi col bluetooth al device.. Default: 1234
unsigned int sign; // Serve per capire se il device ha la eeprom inizializzata o no

// Salva
void push(char choice){
	switch(choice){
		case PWD :
			EEPROM.put(PWDADDR, pwd);
			break;
		case ALARM :
			EEPROM.put(ALARMADDR, sv);
			break;
		case SIGNATURE :
			sign = SIGNVAL; // non é possibile modificarla dall'esterno
			EEPROM.put(SIGNADDR, sign);
			break;
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
		case SIGNATURE :
			EEPROM.get(SIGNADDR, sign);
			break;
	}
}

void initVariables(){
	
	pop(SIGNATURE); // check EEPROM
	
	if(sign != SIGNVAL){ // Se non é inizializzata la inizializzo
		Serial.println("INITIALIZE System Variables with default values...");
		sv = t_sveglia{0, 8, 30, "FAILSAFE", 1, 5};
		pwd = 1234;
		push(ALARM); push(PWD); push(SIGNATURE);
	} else { // se é inizializzata ripristino lo stato precedente delle variabili
		Serial.println("RESUME System Variables from EEPROM to RAM...");
		pop(ALARM);
		pop(PWD);
	}
}



void globalVariablesDump(){
	Serial.println("");
	Serial.println("Dumpo le variabili globali del programma... Enjoy your debug!");
	Serial.println("===========Sveglia===========");
	Serial.print("Attiva: "); Serial.println((sv.active == 1) ? "SI" : "NO");
	Serial.print("Ora: "); Serial.print(sv.ora,DEC); Serial.print(":"); Serial.println(sv.min,DEC);
	Serial.print("Canzone: "); Serial.print(sv.canzone); Serial.print(".wav  Volume: "); Serial.println(sv.volume,DEC);
	Serial.print("Vibrazione: "); Serial.println((sv.vibrazione == 1) ? "SI" : "NO");
	Serial.println("");
	Serial.println("===========Password==========");
	Serial.print("Pwd: ");Serial.println(pwd);
	Serial.println("");
}
