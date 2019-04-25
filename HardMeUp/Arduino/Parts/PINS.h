//========PINS TABLE========

// DIGITAL
#define BTRXPIN 2
#define CLOCKPIN 3
#define BTTXPIN 4 // non collegarlo al bluetooth
#define BUTTON1PIN 7
#define BTVCCPIN 8
#define MOTORSPIN 11
#define LIGHTSPIN 10

//ANALOG
#define FORCE1PIN A2
#define FORCE2PIN A3

// for further informations see the sketch (../Circuit.png)
//==========================


void setPins(){
	// BLUETOOTH
	pinMode(BTVCCPIN, OUTPUT);
	
	// RTC
	pinMode(CLOCKPIN, INPUT_PULLUP);
	
	// BOTTONE
	pinMode(BUTTON1PIN, INPUT);
	
	// MOTORI
	pinMode(MOTORSPIN, OUTPUT);
	
	// LUCI
	pinMode(LIGHTSPIN, OUTPUT); // (si possono usare anche come luci di stato, opzione da attivare tramite impostazione globale da app)
}
