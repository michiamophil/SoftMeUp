#define STANDBYTIME 20000 //20*1000 Tempo di standby 20 secondi
#define FORCETHRESHOLD 550
#define NOISEPRESSUREPHASE1 5
#define NOISEPRESSUREPHASE2 100 // ammette un limite al rumore maggiore perché valuta un numero di iterazioni di molto maggiore

#include <Streaming.h> 

// Il file fornisce un'implementazione trasparente alla routine alarmRoutine() sul sensore di forza che l'utente usa, sia che sia il primo, che il secondo.
// il primo sará disposto in alto e il secondo in basso, entrambi fanno il giro del cuscino su entrambi i lati
//
//   ----------------------------------
//  |                                 |
//  | =============================== |	     <-SENSORE FORZA 1
//  |                                 |
//  |                                 |
//  |                                 |-O    <-BOTTONE
//  |                                 |
//  |                                 |
//  | =============================== |      <-SENSORE FORZA 2
//  |                                 |
//   ----------------------------------
//
// NOTA: la lettura dal sensore viene effettuata solamente durante la fase di RING della sveglia (alarmIsOn)
// 			nel caso in cui l'utente effettua lo snooze della sveglia, fa una piccola pausa (es 1 min) es va in bagno, apre le taparelle e si ricorica a letto, 
//			poi la sveglia suonerá di nuovo, ma si spegnerá subito se non rileva pressione durante la successiva fase di ringing

extern void alarmOff();
extern void alarmOn();

unsigned long nowTimeForThreshold; // settata come variabile globale invece che locale per evitare di istanziarla all'interno di forceSensorPressionCheck()
// per rendere piu veloce il codice di check del sensore di forza

// integra un controllo del rumore
int noiseCounter = 0;

int forceCheck(int pin){
	// DEBUG
	//Serial << analogRead(pin);
	//Serial.println();
	
	return analogRead(pin);
}

bool forceSensorPressionCheck(){
	// si controlla subito se ce pressione e in caso si esce subito
	// (maggior parte dei casi in cui viene chiamata)
	if( forceCheck(FORCE1PIN) > FORCETHRESHOLD || forceCheck(FORCE2PIN) > FORCETHRESHOLD ){
		noiseCounter = 0;
		return true;
	} else {
		noiseCounter++;
	}
	
	// per $NOISETHRESHOLD volte consecutive si é verificato un valore inferiore alla soglia, allora é quasi certo che non si tratti di rumore/un dato sballato
	// e la persona ha alzato la testa dal cuscino
	
		
	// in caso contrario si setta il timer e si resta all'interno della funzione per il tempo necessario a verificare che l'utente non riappoggi la testa

	if (noiseCounter > NOISEPRESSUREPHASE1){
		
		bool pressionIsNotPresent = true;
		nowTimeForThreshold = millis();
		
		alarmOff(); // sospende momentaneamente la sveglia
		
		Serial << "Si é alzata la testa\n";
		//=====================================================================================================================================================
		// ATTENZIONE: l'arduino d'ora in poi entra in uno stato di FREEZE, non verrá controllata alcuna pressione del bottone ne comunicazione bluetooth
		//				é bene che il tempo STANDBYTIME non sia aumentato troppo
		
		noiseCounter = 0; // controllo del rumore anche per verificare che non si verifichi un falso positivo (riuso lo stesso contatore)
		while(pressionIsNotPresent){
			
			if( forceCheck(FORCE1PIN) > FORCETHRESHOLD || forceCheck(FORCE2PIN) > FORCETHRESHOLD ){
				noiseCounter++;
			}
			
			// L'utente é ritornato sul cuscino
			// La sveglia ricomincerá ad andare (solo in hard mode?! e in caso di soft sleep effettua uno snooze)
			// Soluzione implementativa attuale:
			// se entro i 20 secondi si riappoggia la testa la sveglia ricomincia a vibrare (é pur sempre possibile effettuare lo snooze dal tasto e dormire un altro po, se snoozeCount>0, se no entra in HARD MODE
			// in soldoni lascia scegliere ad alarmRoutine() che azione intraprendere automaticamente
			
			if (noiseCounter > NOISEPRESSUREPHASE2) {
				Serial << "Ma ora é di nuovo sul cuscino..!\n";
				alarmOn();
				return true;
			}
			
			// In questo caso posso essere sicuro che l'utente si é alzato
			
			// in questo caso alarmRoutine() sará una funzione brevissima, due check di if.
			// quindi la maggior parte del tempo della cpu sará per rimanere in attesa di azioni dal bottone e dal bluetooth
			// come ad esempio, settare nuovamente un altra sveglia
			if (millis() > nowTimeForThreshold + STANDBYTIME){
				return false;
			}
		}
		//=====================================================================================================================================================	
	}
	
	
	
}

