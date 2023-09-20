// /*Display Code*/
// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_NAU7802.h>
// #include "TLCtest.h"
// #include <TFT_eSPI.h>
// #include <CST816S.h>
// #include <SPI.h>
// #include <chrono>

// TFT_eSPI tft = TFT_eSPI();

// CST816S touch(21, 22, 26, 15); // sda, scl, rst, irq
// String currentGesture;
// bool beenHereBefore = 0;
// bool allow = 0;

// Adafruit_NAU7802 nau;

// void initNAU7802()
// {
//   if (!nau.begin(&Wire1))
//   {
//     Serial.println("Failed to find NAU7802");
//   }
//   Serial.println("Found NAU7802");

//   nau.setGain(NAU7802_GAIN_32);
//   Serial.print("Gain set to 1x");

//   nau.setLDO(NAU7802_EXTERNAL);
//   nau.setRate(NAU7802_RATE_320SPS);

//   // Take 10 readings to flush out readings
//   for (uint8_t i = 0; i < 10; i++)
//   {
//     while (!nau.available())
//       delay(1);
//     nau.read();
//   }
// }

// void initSenseModule()
// {
//   initTLC(0x40, 4); // Initialise TLC LED Driver
//   initNAU7802();    // Initialise the ADC
//   delay(1000);      // Allow time for the initNAU to work,

//   Serial.println("Launch Ambient Lighting");
//   Serial.println(nau.read());
// }

// void printScreen(int input)
// {
//   // tft.fillScreen(TFT_BLACK);
//   tft.fillRect(80, 160, 160, 80, TFT_BLACK);
//   tft.setTextDatum(MC_DATUM); // Sets datum/origin of text to Middle-Centre of the screen
//   tft.drawString("Plastic Type:", tft.width() / 2, 50, 4);
//   tft.drawString(String(input), tft.width() / 2, tft.height() / 2, 8);
// }

// void startingDevicePage()
// {
//   tft.fillScreen(TFT_BLACK);
//   tft.setTextDatum(MC_DATUM); // Sets datum/origin of text to Middle-Centre of the screen
//   tft.drawString("Loading", tft.width() / 2, tft.height() / 2, 4);
// }

// void startPage()
// {
//   tft.fillScreen(TFT_BLACK);
//   tft.setTextDatum(MC_DATUM); // Sets datum/origin of text to Middle-Centre of the screen
//   tft.drawString("Ready to scan!", tft.width() / 2, tft.height() / 2, 4);
//   tft.drawString("PLAS-ID", tft.width() / 2, 30, 2);
//   tft.drawString("Hold down for 2 seconds", tft.width() / 2, (tft.height() / 2) + 30, 2);
//   tft.drawString("to scan", tft.width() / 2, (tft.height() / 2) + 45, 2);
//   // tft.fillCircle(340, tft.height() / 2, tft.width() / 2, TFT_GREEN);
// }

// void printPlasticType(int inputReading)
// {
//   tft.fillRect(0, 240, 160, 80, TFT_BLACK);
//   tft.setTextDatum(MC_DATUM); // Sets datum/origin of text to Middle-Centre of the screen
//   tft.drawString("ADC Reading:", tft.width() / 2, 50, 4);
//   tft.drawString(String(inputReading), tft.width() / 2, tft.height() / 2, 6);
// }

// void flushReadings(uint8_t amount)
// {
//   for (uint8_t i = 0; i < amount; i++)
//   {
//     while (!nau.available())
//       delay(1);
//     nau.read();
//   }
// }

// double getCurrentReading(uint8_t repeats = 1)
// {
//   double adcReadings = 0;
//   flushReadings(5);

//   for (uint8_t i = 0; i < repeats; i++)
//   {
//     while (!nau.available())
//     {
//       delay(1);
//     }
//     adcReadings += nau.read();
//     // Serial.println(adcReadings);
//   }
//   adcReadings = (adcReadings / repeats);

//   // Serial.println(adcReadings);

//   return adcReadings;
// }

// int nauReading()
// {
//   while (!nau.available())
//   {
//     delay(1);
//   }
//   return nau.read();
// }

// void scanAll()
// {
//   // flushReadings(5);
//   int ref = nauReading();
//   Serial.print(ref);
//   Serial.print(",");
//   for (int LED = 0; LED < 8; LED++)
//   {
//     int ref = nauReading();
//     ledOnOff(LED, 1);
//     delay(15);
//     int relReading = nauReading() - ref;
//     Serial.print(relReading);
//     Serial.print(",");
//     ledOnOff(LED, 0);
//     delay(15);
//   }

//   // Get the reflectance values for combinations of the 8 LED's
//   for (int LED = 0; LED < 7; LED++)
//   {
//     int ref = nauReading();
//     ledOnOff(LED, 1);
//     ledOnOff(LED + 1, 1);
//     delay(15);
//     int relReading = nauReading() - ref;
//     Serial.print(relReading);
//     if (LED < 6)
//     {
//       Serial.print(",");
//     }
//     ledOnOff(LED, 0);
//     ledOnOff(LED + 1, 0);
//     delay(15);
//   }
//   Serial.println();
// }

// void setup()
// {
//   Serial.begin(115200);

//   tft.init();
//   delay(500);
//   startingDevicePage();

//   touch.begin();

//   Wire1.begin(32, 33);
//   initSenseModule();

//   pinMode(12, OUTPUT);

//   startPage();
// }

// void loop()
// {
//   // Serial.println(touch.gesture());
//   if (touch.available())
//   {
//     if (!beenHereBefore)
//     {
//       tft.fillScreen(TFT_BLACK);
//       beenHereBefore = 1;
//     }
//     else if (touch.gesture() == "SINGLE CLICK") // && touch.gesture() != currentGesture)
//     {
//       long start_time = millis();
//       scanAll();
//       long end_time = millis();
//       Serial.print("Time Take: ");
//       Serial.println(end_time - start_time);

//       // currentGesture = touch.gesture();
//     }
//     else if (touch.gesture() == "SWIPE UP") // && touch.gesture() != currentGesture)
//     {
//       digitalWrite(12, HIGH);
//       delay(200);
//       digitalWrite(12, LOW);
//       delay(100);
//       digitalWrite(12, HIGH);
//       delay(200);
//       digitalWrite(12, LOW);
//       // currentGesture = touch.gesture();
//     }
//     touch.sleep();
//   }

//   // if(!touch.available())
//   // {
//   //   allow = 1;
//   // }
// }

/*Display Code*/
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NAU7802.h>
#include "TLC.h"
#include "LCD.h"
#include <CST816S.h>

CST816S touch(21, 22, 26, 15); // sda, scl, rst, irq
bool allow = 0;

Adafruit_NAU7802 nau;

long startT = 0;

uint64_t sensor_readings[16];

const uint8_t buttonPin = 17;

void probe(bool probeNumber)
{

  if (!probeNumber)
  {
    startT = millis();
  }
  else
  {
    long stopT = millis();
    Serial.println(stopT - startT);
  }
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

int getNAUReading()
{
  while (!nau.available())
  {
    delay(1);
  }
  return nau.read();
}

void initNAU7802()
{
  if (!nau.begin(&Wire1))
  {
    Serial.println("Failed to find NAU7802");
  }
  nau.setGain(NAU7802_GAIN_32);
  nau.setLDO(NAU7802_EXTERNAL);
  nau.setRate(NAU7802_RATE_320SPS);

  // Take 10 readings to flush out readings
  flushReadings(10);
}

void initSenseModule()
{
  initTLC(0x40, 4); // Initialise TLC LED Driver
  initNAU7802();    // Initialise the ADC
  delay(500);       // Allow time for the initNAU to work,
}

double getAvgReading(uint8_t repeats = 1)
{
  double adcReadings = 0;
  flushReadings(5);

  for (uint8_t i = 0; i < repeats; i++)
  {
    adcReadings += getNAUReading();
  }

  adcReadings = (adcReadings / repeats);

  return adcReadings;
}

void scanWithAllLEDs()
{
  // int ref = getNAUReading();
  // Serial.print(ref);
  // Serial.print(",");
  // sensor_readings[0] = ref;
  for (int LED = 0; LED < 8; LED++)
  {
    int ref = getNAUReading();
    // ledOnOff(LED, 1);
    setBrightness(LED, 0xff);
    delay(15);
    int relReading = getNAUReading() - ref;
    Serial.print(relReading);
    Serial.print(",");
    // ledOnOff(LED, 0);
    setBrightness(LED, 0x00);
    delay(15);
    sensor_readings[LED + 1] = relReading;
  }

  // Get the reflectance values for combinations of the 8 LED's
  for (int LED = 0; LED < 7; LED++)
  {
    int ref = getNAUReading();
    // ledOnOff(LED, 1);
    // ledOnOff(LED + 1, 1);
    setBrightness(LED, 0x7f);
    setBrightness(LED + 1, 0x7f);
    delay(15);
    int relReading = getNAUReading() - ref;
    Serial.print(relReading);
    if (LED < 6)
    {
      Serial.print(",");
    }
    // ledOnOff(LED, 0);
    // ledOnOff(LED + 1, 0);
    setBrightness(LED, 0x00);
    setBrightness(LED + 1, 0x00);
    delay(15);

    sensor_readings[LED + 9] = relReading;
  }
  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Initialising LCD");
  initLCD();

  startingDevicePage();

  Serial.println("Initialising Touchscreen");
  touch.begin();

  Serial.println("Initialising Sense Module");
  Wire1.begin(32, 33);
  initSenseModule();
  setupLEDOutput(2); // This allows the "setLEDBrightness" function. Remove if not in use

  pinMode(12, OUTPUT);
  pinMode(buttonPin, INPUT);

  startPage();
  Serial.println("All good");
}

void loop()
{
  /* The touchscreen registeres a gesture as long as a finger is on the screen. So to avoid registering the same gesture...
  multiple times over we only allow a gesture to register directly after a "NONE" gesture, ensuring it only runs once.*/
  if (touch.gesture() == "NONE")
  {
    allow = 1;
  }

  int buttonState = digitalRead(buttonPin);

  // Run code based on the gesture detected by the touch screen
  if (touch.available() && allow)
  {
    allow = 0;

    if (touch.gesture() == "SINGLE CLICK")
    {
      scanWithAllLEDs();
      delay(500);
      if (Serial.available())
      {
        String receivedData = Serial.readString(); // Read the incoming byte
        printPlasticType(receivedData);
      }
    }
    else if (touch.gesture() == "SWIPE UP")
    {
      failedReading();
      buzz(150);
      delay(50);
      buzz(150);
    }
    else if (touch.gesture() == "SWIPE DOWN")
    {
      Serial.println("close");
    }
  }

  if (buttonState == HIGH)
  {
    // Serial.println("It is working");
    scanWithAllLEDs();
    // delay(500);
    // if (Serial.available())
    // {
    //   String receivedData = Serial.readString(); // Read the incoming byte
    //   printPlasticType(receivedData);
    // }
  }
}