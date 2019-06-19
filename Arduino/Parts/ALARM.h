#define ALARMDURATION 300000 //5*60*1000 5 minuti
#define UNLIMITEDSNOOZE 99 //Se snoozeCount vale $UNLIMITEDSNOOZE, non entra mai in hard mode)

extern void motorsOn();
extern void motorsOff();
extern void motorsChangeState();

extern void lightsOn();
extern void lightsOff();
extern void lightsChangeState();

extern bool forceSensorPressionCheck();
extern void setRTCInterruptAlarm();
extern int noiseCounter;
extern void BPrint(String msg);
extern void BPrintDec(int n);
extern void BPrintNoNl(String msg);
extern void printTime(time_t t);

bool alarmIsOn;

unsigned long alarmDurationCheck;
bool snoozeIsPending;



void alarmSetFromSettings(){
		if(sv.thisAlarmIsActive){
			setRTCInterruptAlarm(sv.ora, sv.min, 0);
		}
}

void alarmOn(){
	motorsOn();
	lightsOn();
}

void alarmOff(){
	motorsOff();
	lightsOff();
}

void alarmRoutineSetNewAlarm(){
	time_t tm = RTC.get();
	if(sv.snoozeCount != UNLIMITEDSNOOZE){
		sv.snoozeCount--;
	}
	// DEBUG
	printTime(RTC.get());
	BPrintNoNl("N7 "); BPrintDec((int) sv.snooseTimer); BPrintNoNl(" - ");BPrintDec((int) sv.snoozeCount); BPrint("");
	
	setRTCInterruptAlarm(hour(tm), minute(tm) + (int)sv.snooseTimer, second(tm));
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
	BPrint("Alarm EXIT\n");
	alarmOff();
	button1.setEventHandler(button1GenericHandle);
	alarmIsOn = false;
}


void alarmRoutine(){
	
	// Controllo se l'RTC genera l'evento ALARM
	if (!digitalRead(CLOCKPIN)){
		
		BPrint("Alarm"); // DEBUG
		
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
			return;
		};
		
		
		motorsChangeState();
		lightsChangeState();
	}
	
}
