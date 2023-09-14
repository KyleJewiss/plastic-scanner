/*Display Code*/
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NAU7802.h>
#include "TLCtest.h"
#include <TFT_eSPI.h>
#include <CST816S.h>
#include <SPI.h>
#include <chrono>

TFT_eSPI tft = TFT_eSPI();

CST816S touch(21, 22, 26, 15); // sda, scl, rst, irq
String currentGesture;
bool beenHereBefore = 0;
bool allow = 0;

Adafruit_NAU7802 nau;

void initNAU7802()
{
  if (!nau.begin(&Wire1))
  {
    Serial.println("Failed to find NAU7802");
  }
  Serial.println("Found NAU7802");

  nau.setGain(NAU7802_GAIN_32);
  Serial.print("Gain set to 1x");

  nau.setLDO(NAU7802_EXTERNAL);
  nau.setRate(NAU7802_RATE_320SPS);

  // Take 10 readings to flush out readings
  for (uint8_t i = 0; i < 10; i++)
  {
    while (!nau.available())
      delay(1);
    nau.read();
  }
}

void initSenseModule()
{
  initTLC(0x40, 4); // Initialise TLC LED Driver
  initNAU7802();    // Initialise the ADC
  delay(1000);      // Allow time for the initNAU to work,

  Serial.println("Launch Ambient Lighting");
  Serial.println(nau.read());
}

void printScreen(int input)
{
  // tft.fillScreen(TFT_BLACK);
  tft.fillRect(80, 160, 160, 80, TFT_BLACK);
  tft.setTextDatum(MC_DATUM); // Sets datum/origin of text to Middle-Centre of the screen
  tft.drawString("Plastic Type:", tft.width() / 2, 50, 4);
  tft.drawString(String(input), tft.width() / 2, tft.height() / 2, 8);
}

void startingDevicePage()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM); // Sets datum/origin of text to Middle-Centre of the screen
  tft.drawString("Loading", tft.width() / 2, tft.height() / 2, 4);
}

void startPage()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM); // Sets datum/origin of text to Middle-Centre of the screen
  tft.drawString("Ready to scan!", tft.width() / 2, tft.height() / 2, 4);
  tft.drawString("PLAS-ID", tft.width() / 2, 30, 2);
  tft.drawString("Hold down for 2 seconds", tft.width() / 2, (tft.height() / 2) + 30, 2);
  tft.drawString("to scan", tft.width() / 2, (tft.height() / 2) + 45, 2);
  // tft.fillCircle(340, tft.height() / 2, tft.width() / 2, TFT_GREEN);
}

void printPlasticType(int inputReading)
{
  tft.fillRect(0, 240, 160, 80, TFT_BLACK);
  tft.setTextDatum(MC_DATUM); // Sets datum/origin of text to Middle-Centre of the screen
  tft.drawString("ADC Reading:", tft.width() / 2, 50, 4);
  tft.drawString(String(inputReading), tft.width() / 2, tft.height() / 2, 6);
}

void flushReadings(uint8_t amount)
{
  for (uint8_t i = 0; i < amount; i++)
  {
    while (!nau.available())
      delay(1);
    nau.read();
  }
}

double getCurrentReading(uint8_t repeats = 1)
{
  double adcReadings = 0;
  flushReadings(5);

  for (uint8_t i = 0; i < repeats; i++)
  {
    while (!nau.available())
    {
      delay(1);
    }
    adcReadings += nau.read();
    // Serial.println(adcReadings);
  }
  adcReadings = (adcReadings / repeats);

  // Serial.println(adcReadings);

  return adcReadings;
}

int nauReading()
{
  while (!nau.available())
  {
    delay(1);
  }
  return nau.read();
}

void scanAll()
{
  // flushReadings(5);
  int ref = nauReading();
  Serial.print(ref);
  Serial.print(",");
  for (int LED = 0; LED < 8; LED++)
  {
    int ref = nauReading();
    ledOnOff(LED, 1);
    delay(15);
    int relReading = nauReading() - ref;
    Serial.print(relReading);
    Serial.print(",");
    ledOnOff(LED, 0);
    delay(15);
  }

  // Get the reflectance values for combinations of the 8 LED's
  for (int LED = 0; LED < 7; LED++)
  {
    int ref = nauReading();
    ledOnOff(LED, 1);
    ledOnOff(LED + 1, 1);
    delay(15);
    int relReading = nauReading() - ref;
    Serial.print(relReading);
    if (LED < 6)
    {
      Serial.print(",");
    }
    ledOnOff(LED, 0);
    ledOnOff(LED + 1, 0);
    delay(15);
  }
  Serial.println();
}

void setup()
{
  Serial.begin(115200);

  tft.init();
  delay(500);
  startingDevicePage();

  touch.begin();

  Wire1.begin(32, 33);
  initSenseModule();

  pinMode(12, OUTPUT);

  startPage();
}

void loop()
{
  // Serial.println(touch.gesture());
  if (touch.available())
  {
    if (!beenHereBefore)
    {
      tft.fillScreen(TFT_BLACK);
      beenHereBefore = 1;
    }
    if (touch.gesture() == "SINGLE CLICK") // && touch.gesture() != currentGesture)
    {
      long start_time = millis();
      scanAll();
      long end_time = millis();
      Serial.print("Time Take: ");
      Serial.println(end_time - start_time);

      // currentGesture = touch.gesture();
    }
    if (touch.gesture() == "SWIPE UP") // && touch.gesture() != currentGesture)
    {
      digitalWrite(12, HIGH);
      delay(200);
      digitalWrite(12, LOW);
      delay(100);
      digitalWrite(12, HIGH);
      delay(200);
      digitalWrite(12, LOW);
      // currentGesture = touch.gesture();
    }
    touch.sleep();
  }

  // if(!touch.available())
  // {
  //   allow = 1;
  // }
}