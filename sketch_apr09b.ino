// DS3231/DS3232 Alarm Example Sketch #7
//
// Set Alarm 2 to occur at a given time hh:mm.
// Configure the RTC INT/SQW pin to be asserted when alarm match occurs.
// Detect the alarm by polling the INT/SQW pin.
//
// Hardware:
// Arduino Uno, DS3231 RTC.
// Connect RTC SDA to Arduino pin A4.
// Connect RTC SCL to Arduino pin A5.
// Connect RTC INT/SQW to Arduino pin 2.
//

#include <DS3232RTC.h>        // https://github.com/JChristensen/DS3232RTC
#include <Streaming.h>        // http://arduiniana.org/libraries/streaming/

#include "RTCLIB.h"

// pin definitions
const uint8_t alarmInput(2);

void setup()
{
    Serial.begin(115200);
    
    initializeRTC();
    initializeRTCAlarms();
    setRTCTime(2019,4,9,17,52,50); // set custom time
    pinMode(2, INPUT_PULLUP);    
    setRTCInterruptAlarm(17,53);
}

void loop()
{
    // check to see if the SQW pin is low, i.e. an alarm has occurred
    if ( !digitalRead(alarmInput) ) {
        RTC.alarm(ALARM_2);    // reset the alarm flag
        Serial << "ALARM_2\n";
    }

    // print the time when it changes
    static time_t tLast;
    time_t t = RTC.get();
    if (t != tLast) {
        tLast = t;
        printDateTime(t);
        Serial.println(getDateTimeString(t));
    }

    
}
