//========PINS TABLE========

// DIGITAL

// PIN 2 SDA
// PIN 3 SCL
#define CLOCKPIN 4 // SQW RTC

#define BUTTON1PIN 5

#define BTSTATUSVCCPIN 7

#define BTRXPIN 8 // RDD del bluetooth con voltage divider
#define BTTXPIN 9

#define BTVCCPIN 10 // VCC BLUETOOTH
#define LIGHTSPIN 11
#define MOTORSPIN 12


//ANALOG
#define FORCE1PIN A5	// IT HAS RELATED THRESHOLD1
#define FORCE2PIN A4	// IT HAS RELATED THRESHOLD2


// for further informations see the sketch (../Circuit.png)
//==========================
void setPins(){
	// BLUETOOTH
	pinMode(BTVCCPIN, OUTPUT);
	pinMode(BTSTATUSVCCPIN, OUTPUT);
	
	// RTC
	pinMode(CLOCKPIN, INPUT_PULLUP);
	
	// BOTTONE
	pinMode(BUTTON1PIN, INPUT);
	
	// MOTORI
	pinMode(MOTORSPIN, OUTPUT);
	
	// LUCI
	pinMode(LIGHTSPIN, OUTPUT); 
}
