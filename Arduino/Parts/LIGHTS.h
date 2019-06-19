#define LUCE_FISSA 3
#define BLINK_LENTO 2
#define BLINK_RAPIDO 1

#define DELAY_BLINK_LENTO 1000 // millisecondi
#define DELAY_BLINK_RAPIDO 400

extern t_sveglia sv;
bool lightsAreOn;

#define LIGHTSTHRESHOLD 50

void lightsOn(){
	if (sv.luci != 0){
		lightsAreOn = true;
		analogWrite(LIGHTSPIN, LIGHTSTHRESHOLD+((255-LIGHTSTHRESHOLD)/8)*(sv.luci-1));
	}
}

void lightsOff(){
	lightsAreOn = false;
	digitalWrite(LIGHTSPIN, LOW);
}


unsigned long previousTimeLights;
void lightsChangeState(){
	if (sv.tipo_luci != LUCE_FISSA){
		
		if(sv.tipo_luci == BLINK_LENTO){
			if(millis() > previousTimeLights + DELAY_BLINK_LENTO){
				previousTimeLights = millis();
				// CHANGE STATE
				if( lightsAreOn ) {
					lightsOff();
					return;
				} else {
					lightsOn();
					return;
				}
			}
			
		} else if (sv.tipo_luci == BLINK_RAPIDO){
			if(millis() > previousTimeLights + DELAY_BLINK_RAPIDO){
				previousTimeLights = millis();
				// CHANGE STATE
				if( lightsAreOn ) {
					lightsOff();
					return;
				} else {
					lightsOn();
					return;
				}
			}
		}
	}
}
