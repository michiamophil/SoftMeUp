/* Olivier + Simone code for the state manager
 * 
 */
 
#include <DS3232RTC.h>        // https://github.com/JChristensen/DS3232RTC
#include <Streaming.h>        // http://arduiniana.org/libraries/streaming/

//<TimeLib.h> to be downloaded for RTC

#include "RTCLIB.h"
#include "pitches.h"

const int pressureInPin = A0; //analog input pin that the pressure sensor/potentiometer is attached to
const int vibrationPin = 8; //digital output pin for setting on/off the vibration motor
const int musicPin = 9; //analog output pin
const int clockPin = 2; //input pin for RTC and checking alarm time 


int prev = 0; 
int adesso = 0;
int oldTime = 0;
int wakeUpTime=5*1000;
int pressureValue=0;
int offTime = 1*60*1000;
bool wakeUp=0;



// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};




void setup() {
  
  pinMode(clockPin, INPUT_PULLUP);
  pinMode(musicPin, OUTPUT);
  pinMode(vibrationPin, OUTPUT);
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  initializeRTC();
  initializeRTCAlarms();
  setRTCTime(2019,4,9,17,52,50); // set custom time
  setRTCInterruptAlarm(17,53);
  
}

void loop() {
  if ( !digitalRead(clockPin) ) {
    RTC.alarm(ALARM_2);    // reset the alarm flag
    wakeUp=1;
  
    while(wakeUp){
          
    // read the analog in value:
    pressureValue = analogRead(pressureInPin);
  
    // print the results to the Serial Monitor:
    Serial.print("\nsensor = ");
    Serial.print(pressureValue);
  
    delay(2);
      
      if (pressureValue > 511) { //pressure sensor is on (at the alarm time)
  
        adesso = 1;          
  
        Serial.print(" adesso = ");
        Serial.print(adesso);
        Serial.print(" previous = ");
        Serial.print(prev);
        Serial.print(" oldTime = ");
        Serial.print(oldTime);
        
        // condizione per capire se é passato il tempo
        if (adesso != prev){
          if(millis() > oldTime + wakeUpTime && oldTime != 0) {
            Serial.print("/n time adesso = ");
            Serial.print(millis());
            Serial.print(" oldTime = ");
            Serial.print(oldTime);
            Serial.print(" difference = ");
            Serial.print(millis()-oldTime);
          
            wakeUp = 0;
            break;
          }
          
          prev = adesso;
          oldTime = millis();
  
        digitalWrite(vibrationPin, HIGH); //set the vibration motor pin on
        digitalWrite(musicPin,HIGH); // set the music pin on
  
          
        }
        
        //digitalWrite(vibrationPin, HIGH); //set the vibration motor pin on
        //digitalWrite(musicPin,HIGH); // set the music pin on
  
        
        for (int thisNote = 0; thisNote < 8; thisNote++) {
  
          // to calculate the note duration, take one second divided by the note type.
          //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
          int noteDuration = 1000 / noteDurations[thisNote];
          tone(8, melody[thisNote], noteDuration);
  
          // to distinguish the notes, set a minimum time between them.
          // the note's duration + 30% seems to work well:
          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);
          // stop the tone playing:
          noTone(8);
        }
      
      }
  
      if (pressureValue <= 511) {
  
        adesso = 0;    
  
        Serial.print(" adesso = ");
        Serial.print(adesso);
        Serial.print(" previous = ");
        Serial.print(prev);
        Serial.print(" oldTime = ");
        Serial.print(oldTime);
        
        if (millis() > oldTime + offTime) {
			
			wakeup = 0;
		
		}
		
  
        
        //digitalWrite(vibrationPin, LOW); //set the vibration motor pin off
        //digitalWrite(musicPin,LOW); // set the music pin off
        
        if (adesso != prev){
          prev = adesso;
          oldTime = millis();
  
        digitalWrite(vibrationPin, LOW); //set the vibration motor pin off
        digitalWrite(musicPin,LOW); // set the music pin off
          
        }
      }
    }
  }
}
  
