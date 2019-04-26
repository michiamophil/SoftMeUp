#define ALARMDURATION 300000 //5*60*1000 5 minuti
#define UNLIMITEDSNOOZE 99 //Se snoozeCount vale $UNLIMITEDSNOOZE, non entra mai in hard mode)

extern void motorsOn();
extern void motorsOff();
extern void lightsOn();
extern void lightsOff();
extern bool forceSensorPressionCheck();
extern void setRTCInterruptAlarm();
extern int noiseCounter;

bool alarmIsOn = false;

unsigned long alarmDurationCheck;
bool snoozeIsPending = false;



void alarmSetFromSettings(){
		if(sv.thisAlarmIsActive){
			setRTCInterruptAlarm(sv.ora, sv.min);
		}
}

void alarmOn(){
	
	if(sv.vibrazione){
			motorsOn();
		}
		
	if(sv.luci){
			lightsOn();
		}
}

void alarmOff(){
	
	if(sv.vibrazione){
			motorsOff();
		}
		
	if(sv.luci){
			lightsOff();
		}
}

void alarmRoutineSetNewAlarm(){
	time_t tm = RTC.get();
	if(sv.snoozeCount != UNLIMITEDSNOOZE){
		sv.snoozeCount--;
	}
	// DEBUG
	printTime(RTC.get());
	Serial << "La prossima sveglia suona tra " << (int) sv.snooseTimer << " min\n";
	Serial << "(Snooze rimasti: " << (int) sv.snoozeCount << ")\n"; 
	
	setRTCInterruptAlarm(hour(tm), minute(tm) + (int)sv.snooseTimer);
}



// Richiamata dal bottone (STATO SNOOZE) o dopo che il cuscino ha vibrato per il tempo massimo impostato: ALARMDURATION in alarmRoutine()
void alarmRoutineAlarmSnooze(){
	alarmRoutineSetNewAlarm(); // L'allarme viene solo posticipato
	alarmOff();
	snoozeIsPending = true;
	alarmIsOn = false;
}

// Richiamata dal bottone (STATO HARD PRESSIONE LUNGA) SNOOZE solo su impostazione app? in caso é facile da aggiungere
void alarmRoutineAlarmEXIT(){
	// Viene chiamata questa funzione solamente quando siamo sicuri che l'utente si é svegliato
	Serial << "=======Alarm EXIT!=======\n";
	alarmOff();
	button1.setEventHandler(button1GenericHandle);
	alarmIsOn = false;
}


void alarmRoutine(){
	
	// Controllo se l'RTC genera l'evento ALARM
	if (!digitalRead(CLOCKPIN)){
		
		Serial << "Sveglia Attivata!\n"; // DEBUG
		
		alarmIsOn = true;
		snoozeIsPending = false;
		alarmDurationCheck = millis();
		
		alarmOn();
		
		if(!sv.snoozeCount){ // Se snoozeCount = 0 allora é nello stato hard wake up
			button1.setEventHandler(button1HardHandle); // Il bottone entra nello stato HARD
		} else {
			button1.setEventHandler(button1SnoozeHandle); // Il bottone entra nello stato SNOOZE
		}
		
		noiseCounter = 0;// per sveglie ripetute, risetta il noise counter a zero
		
		RTC.alarm(ALARM_2);    // reset the alarm flag
	}		
	
	if (alarmIsOn){
		
		// Se la sveglia ha gia suonato per es. 5 min (IN MODALITA HARD SUONA ALL'INFINITO SE CE PRESSIONE DAL SENSORE) a meno di una long press del bottone (SU CONFIGURAZIONE APP) 
		if (millis() > (alarmDurationCheck + ALARMDURATION)){
			if(sv.snoozeCount){
				alarmRoutineAlarmSnooze(); // Faccio lo snooze della sveglia (SOLO NELLA MODALITA SOFT)
			}
		}
		
		if(forceSensorPressionCheck() == false){	// Se non cé stata pressione sul cuscino per un periodo di tempo (FORCETHRESHOLD, definita in FORCESENSORS)
			// allora vuol dire che la persona si é svegliata ed esco dalla routine
			alarmRoutineAlarmEXIT();
		};
	}
	
}
