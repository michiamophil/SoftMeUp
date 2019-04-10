#include <HardwareSerial.h>
#include <DS3232RTC.h>        // https://github.com/JChristensen/DS3232RTC
#include <Streaming.h>        // http://arduiniana.org/libraries/streaming/

void setRTC(int a) {
  Serial.println("LAB");
}

bool initializeRTC() {
  setSyncProvider(RTC.get);
  Serial.print("RTC Sync");
  if (timeStatus() != timeSet) {
    Serial.print(" FAILED!!!");
    Serial.println("");
    return false;
  }
  Serial.println("");
  return true;

}

void initializeRTCAlarms() {
  // initialize the alarms to known values, clear the alarm flags, clear the alarm interrupt flags
  RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
  RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 0, 1);
  RTC.alarm(ALARM_1);
  RTC.alarm(ALARM_2);
  RTC.alarmInterrupt(ALARM_1, false);
  RTC.alarmInterrupt(ALARM_2, false);
  RTC.squareWave(SQWAVE_NONE);
}

void setRTCTime(int y, int mo, int d, int h, int mi, int s) {
  tmElements_t tm;
  tm.Second = s;
  tm.Minute = mi;
  tm.Hour = h;
  tm.Day = d;
  tm.Month = mo;
  tm.Year = y - 1970;      // tmElements_t.Year is the offset from 1970
  RTC.write(tm);
}

void setRTCInterruptAlarm(int h, int mi) {
  RTC.setAlarm(ALM2_MATCH_HOURS, 0, mi, h, 0);
  // clear the alarm flags
  RTC.alarm(ALARM_1);
  RTC.alarm(ALARM_2);
  // configure the INT/SQW pin for "interrupt" operation (disable square wave output)
  RTC.squareWave(SQWAVE_NONE);
  // enable interrupt output for Alarm 2 only
  RTC.alarmInterrupt(ALARM_1, false);
  RTC.alarmInterrupt(ALARM_2, true);
}

void printDateTime(time_t t) {
  Serial << _DEC(year(t)) << "-";
  Serial << ((month(t)<10) ? "0" : "") << _DEC(month(t)) << "-";
  Serial << ((day(t)<10) ? "0" : "") << _DEC(day(t)) << "T";
  Serial << ((hour(t)<10) ? "0" : "") << _DEC(hour(t)) << ':';
  Serial << ((minute(t)<10) ? "0" : "") << _DEC(minute(t)) << ':';
  Serial << ((second(t)<10) ? "0" : "") << _DEC(second(t));
  Serial.println();
}

String getDateTimeString(time_t t) {
  String s = "";
  s+= year(t);
  s+= "-";

  if( month(t) < 10) {
    s+="0";
  }
  s+= month(t);
  s+= "-";

  if( day(t) < 10) {
    s+="0";
  }
  s+= day(t);
  s+= "T";

  if(hour(t) < 10) {
    s+= "0";
  }
  s+= hour(t);
  s+= ":";

  if(minute(t) < 10) {
    s+= "0";
  }
  s+= minute(t);
  s+= ":";
  if(second(t) < 10) {
    s+="0";
  }
  s+=second(t);
  return s;
}
