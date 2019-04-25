#define SERIALBAUD 19200
#define BLUETOOTHBAUD 9600

#include "Parts/PINS.H"
#include "Parts/VARSLIB.H"
#include "Parts/BUTTONS.H"
#include "Parts/RTCLIB.H"
#include "Parts/ALARM.H"
#include "Parts/MOTORS.H"
#include "Parts/LIGHTS.H"
#include "Parts/FORCESENSORS.H"
#include "Parts/BLUETOOTH.H"


//==============TODO==============
// in MOTOR.H e LIGHTS.H usare una tensione variabile invece che un HIGH LOW
// Inoltre in futuro per far funzionare i componenti del cuscino é necessario usare transistor IRL

// Cé la predisposizione per poter settare una password, serve settare una password?

// da implementare: If snoozeIsPending -> longPress on Button (GENERIC STATE) will exit from alarm.
//================================

void setup(){
	Serial.begin(SERIALBAUD); while (!Serial) {;}
  
	setPins();
	initVariables();
	
	initButton1();
	
	bluetoothChangeState(); // Il bluetooth inizialmente si trova spento (bluetoothIsActive é settata di default a false), quindi richiamando la funzione (che di solito é richiamata dal bottone nello stato GENERIC) il bluetooth si accende e inizializza
  
	// IMPORTANTE initVariables() deve essere eseguito prima di initRTC()
	initRTC(); // inizializza L'RTC e imposta la sveglia se é presente nei dati della EEPROM
}


void loop()
{
	alarmRoutine(); // al suo interno ce forceSensorCheck(), é veloce e non tiene impegnata la cpu
	// che deve poter gestire un evento del bottone (che ha check sincrono, non Interrupt) e del bluetooth
		
	button1.check();
	
	bluetoothCheck();
}



