#include <Arduino.h>
#include "sub.h"

void setup()
{
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.init();
  lcd.backlight();
  //====================================================
  lcd.createChar(0, customChar); // LOOOK
  //====================================================
  lcd.setCursor(6, 0);
  lcd.print(F("LAPAN"));
  lcd.setCursor(0, 1);
  lcd.print(F("Smkn 2 Pontianak"));
  delay(2000);
  lcd.clear();

#ifndef ESP8266
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB
#endif

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1)
      delay(10);
  }

  if (!rtc.isrunning())
  {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}
void loop()
{
  lcd.clear();
  BacaJarak();
  printDate();
  LCDDate();
  delay(5000);
}
