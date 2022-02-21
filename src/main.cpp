#include "RTClib.h"
#include <Wire.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>

#define trigPin 15
#define echoPin 4

long duration;
uint8_t distance;

LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS1307 rtc;
void LCDDate();
void printDate();
void BacaJarak();
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
byte customChar[] = {
    B00000,
    B00000,
    B00000,
    B00001,
    B00011,
    B10110,
    B11100,
    B01000};

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
/*
void loop()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000); // wait 5 seconds for next scan
}
*/
void loop()
{
  lcd.clear();
  BacaJarak();
  printDate();
  LCDDate();
  delay(5000);
}

void printDate()
{
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print('\t');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
}
void LCDDate()
{
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print(now.hour());
  lcd.print(':');
  if (now.minute() < 10)
  {
    lcd.print("0");
    lcd.print(now.minute());
  }
  else
  {
    lcd.print(now.minute());
  }
  lcd.setCursor(8, 0);
  lcd.print(now.day());
  lcd.print('/');
  if (now.month() < 10)
  {
    lcd.print("0");
    lcd.print(now.month());
  }
  else
  {
    lcd.print(now.month());
  }
  lcd.print('/');
  lcd.print(now.yr());
  lcd.setCursor(0, 1);
  lcd.print(F("jarak: "));
  lcd.setCursor(7, 1);
  lcd.print(distance);
  lcd.setCursor(10, 1);
  lcd.print(F("cm"));
}
void BacaJarak()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
}