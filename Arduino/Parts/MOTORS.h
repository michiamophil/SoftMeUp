#define VIBRAZIONE_FISSA 3
#define IMPULSO_LENTO 2
#define IMPULSO_RAPIDO 1

#define DELAY_IMPULSO_LENTO 800 // millisecondi
#define DELAY_IMPULSO_RAPIDO 400

#define VIBRAZIONETHRESHOLD 50

extern t_sveglia sv;
bool motorsAreOn;

void motorsOn(){
	if(sv.vibrazione != 0){
		motorsAreOn = true;
		analogWrite(LIGHTSPIN, VIBRAZIONETHRESHOLD+((255-VIBRAZIONETHRESHOLD)/8)*(sv.vibrazione-1));
	}
}

void motorsOff(){
	motorsAreOn = false;
	digitalWrite(MOTORSPIN, LOW);
}


unsigned long previousTimeVibration;
void motorsChangeState(){
	if (sv.tipo_vibrazione != VIBRAZIONE_FISSA){
		
		if(sv.tipo_vibrazione == IMPULSO_LENTO){
			if(millis() > previousTimeVibration + DELAY_IMPULSO_LENTO){
				previousTimeVibration = millis();
				// CHANGE STATE
				if( motorsAreOn ) {
					motorsOff();
					return;
				} else {
					motorsOn();
					return;
				}
			}
			
		} else if (sv.tipo_vibrazione == IMPULSO_RAPIDO){
			if(millis() > previousTimeVibration + DELAY_IMPULSO_RAPIDO){
				previousTimeVibration = millis();
				// CHANGE STATE
				if( motorsAreOn ) {
					motorsOff();
					return;
				} else {
					motorsOn();
					return;
				}
			}
		}
	}
}
