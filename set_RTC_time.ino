#include "RTClib.h"

RTC_DS1307 rtc;

void setup () 
{
  Serial.begin(9600);

  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println("Time is set: ");
   
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print("         ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    Serial.println();
}

void loop () 
{
    delay(5000);
}
