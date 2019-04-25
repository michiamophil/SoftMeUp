#define LONGPRESSDELAY 3000

#include <AceButton.h>
#include <Streaming.h>  
//#include "PINS.H"

using namespace ace_button;

AceButton button1(BUTTON1PIN);

void handleEvent(AceButton*, uint8_t, uint8_t);

extern bool alarmIsOn;
extern bool snoozeIsPending;
extern void alarmRoutineAlarmSnooze();
extern void alarmRoutineAlarmEXIT();
extern void bluetoothChangeState();
extern void initializeRTCAlarms();

extern t_sveglia sv; 



void button1SnoozeHandle(AceButton* /* button */, uint8_t eventType,
    uint8_t /* buttonState */) {
  switch (eventType) {
	 case AceButton::kEventLongPressed:
	{
		if(snoozeIsPending){ // La pressione prolungata del bottone in snooze state disattiva la sveglia
			Serial << "LONG PRESS (SNOOZE STATE) - disattivo la sveglia\n";
			snoozeIsPending = false;
			initializeRTCAlarms(); // rimuovo la sveglia
			alarmRoutineAlarmEXIT();
		}
		break;
	}
    case AceButton::kEventPressed: {
      //Serial << "Bottone Rilasciato";
      //Serial.println();
		if(!snoozeIsPending){
			if(sv.snoozeCount){
				alarmRoutineAlarmSnooze(); // Faccio lo snooze della sveglia (SOLO NELLA MODALITA SOFT)
			}
		}
		break;
	}
    case AceButton::kEventReleased: {
      Serial << "Bottone Premuto (SNOOZE STATE)";
      Serial.println();
      break;
	}
  }
}

void button1HardHandle(AceButton* /* button */, uint8_t eventType,
    uint8_t /* buttonState */) {
  switch (eventType) {
    case AceButton::kEventLongPressed:
	{
			if(settingsButtonHardStateLongPressOn){
				Serial << "LONG PRESS (HARD MODE STATE) - disattivo la sveglia\n";
				alarmRoutineAlarmEXIT();
			} else {				
				Serial << "Impostazione disabilitata\n";
			}
			break;
	}
  }
}

void button1GenericHandle(AceButton* /* button */, uint8_t eventType,
    uint8_t /* buttonState */) {
  switch (eventType) {
    //case AceButton::kEventPressed:
      //Serial << "Bottone Premuto 1 (GENERIC STATE)\n";
      //break;
	case AceButton::kEventReleased:
		Serial << "Bottone Premuto (GENERIC STATE)\n";
		bluetoothChangeState();
		break;
  }
}

void initButton1(){
	button1.setEventHandler(button1GenericHandle);
	
	ButtonConfig* config = button1.getButtonConfig();
	config->setFeature(ButtonConfig::kFeatureLongPress);
	//ButtonConfig::setFeature(kFeatureLongPress);
	config->setLongPressDelay(LONGPRESSDELAY);
}



