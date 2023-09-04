

// /* Test everything together **************************************/
// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_NAU7802.h>
// #include "TLCtest.h"

// #define TLC_ADD 0x40 // I2C address of the TLC59108
// #define BUTTON_PIN 14
// #define R_PIN 32 // Uno: Pin2 ESP32: Pin32
// double maxValues[8];
// bool buttonState = 0;
// int lowest = 0;
// int highest = 0;

// Adafruit_NAU7802 nau;

// void initNAU7802()
// {
//   if (!nau.begin())
//   {
//     Serial.println("Failed to find NAU7802");
//   }
//   Serial.println("Found NAU7802");

//   nau.setGain(NAU7802_GAIN_1);
//   Serial.print("Gain set to 1x");

//   // Take 10 readings to flush out readings
//   for (uint8_t i = 0; i < 10; i++)
//   {
//     while (!nau.available())
//       delay(1);
//     nau.read();
//   }

//   nau.setRate(NAU7802_RATE_10SPS);

//   // while (!nau.calibrate(NAU7802_CALMOD_INTERNAL))
//   // {
//   //   Serial.println("Failed to calibrate internal offset, retrying!");
//   //   delay(1000);
//   // }
//   // Serial.println("Calibrated internal offset");

//   // while (!nau.calibrate(NAU7802_CALMOD_OFFSET))
//   // {
//   //   Serial.println("Failed to calibrate system offset, retrying!");
//   //   delay(1000);
//   // }
//   // Serial.println("Calibrated system offset");
// }

// void flushReadings(int amount)
// {
//   for (uint8_t i = 0; i < amount; i++)
//   {
//     while (!nau.available())
//       delay(1);
//     nau.read();
//   }
//   delay(100);
// }

// double getCurrentReading(int repeats)
// {
//   int32_t adcReadings = 0;
//   double avgAdcReadings = 0;

//   // Read and record next 3
//   for (uint8_t i = 0; i < repeats; i++)
//   {
//     while (!nau.available())
//     {
//       delay(1);
//     }
//     adcReadings += nau.read();
//     // Serial.println(adcReadings);
//   }

//   avgAdcReadings = adcReadings / repeats;

//   // Serial.println(avgAdcReadings);
//   return avgAdcReadings;
// }

// void scanAll()
// {
//   flushReadings(10);
//   // delay(1000);
//   // double referenceLightLevel = getCurrentReading(3);
//   // Get the reflectance values for the 8 LED's
//   for (int LED = 0; LED < 8; LED++)
//   {
//     double referenceLightLevel = getCurrentReading(3);
//     setBrightness(LED, 0xff);
//     delay(100);
//     int absoluteLightLevel = (getCurrentReading(3) - referenceLightLevel);
//     Serial.print(absoluteLightLevel);

//     Serial.print(",");
//     setBrightness(LED, 0x00);
//     delay(100);
//   }

//   // Get the reflectance values for combinations of the 8 LED's
//   for (int LED = 0; LED < 7; LED++)
//   {
//     double referenceLightLevel = getCurrentReading(3);
//     setBrightness(LED, 0x7F);
//     setBrightness(LED + 1, 0x7F);
//     delay(100);
//     int absoluteLightLevel = (getCurrentReading(3) - referenceLightLevel);
//     Serial.print(absoluteLightLevel);
//     if (LED < 6)
//     {
//       Serial.print(",");
//     }
//     setBrightness(LED, 0x00);
//     setBrightness(LED + 1, 0x00);
//     delay(100);
//   }
//   Serial.println();
// }

// void relScan()
// {
//   delay(100);
//   for (int LED = 0; LED < 8; LED++)
//   {
//     double referenceLightLevel = getCurrentReading(10);
//     Serial.println(referenceLightLevel);

//     setBrightness(LED, 0xff);
//     delay(500);
//     double absoluteLightLevel = (getCurrentReading(10) - referenceLightLevel) / maxValues[LED];
//     // Serial.print(getCurrentReading(10));
//     Serial.print(absoluteLightLevel, 5);

//     Serial.print(",");
//     setBrightness(LED, 0x00);
//     delay(100);
//   }
// }

// void intensityOverTime()
// {
//   double referenceLightLevel = getCurrentReading(3);

//   for (int PWM = 0; PWM < 128; PWM++) // Set to 1/3 Brightness so it doesn't saturate
//   {
//     for (int LED = 0; LED < 8; LED++)
//     {
//       setBrightness(LED, PWM);
//     }
//     delay(10);
//     // Serial.println(getCurrentReading(3)-referenceLightLevel);
//     Serial.println(nau.read() - referenceLightLevel);
//     delay(10);
//   }

//   for (int PWM = 126; PWM >= 0; PWM--)
//   {
//     for (int LED = 0; LED < 8; LED++)
//     {
//       setBrightness(LED, PWM);
//     }
//     delay(10);
//     // Serial.println(getCurrentReading(3)-referenceLightLevel);
//     Serial.println(nau.read() - referenceLightLevel);
//     delay(10);
//   }
//   initTLC(TLC_ADD, R_PIN); // Initialise TLC LED Driver
//   setupLEDOutput(2);       // Set the mode of the LED Driver
//   delay(100);
// }

// void setup()
// {
//   Serial.begin(115200);
//   Wire.begin(); // Initialize I2C communication
//   Wire.setClock(400000);
//   pinMode(BUTTON_PIN, INPUT); // Set the button pin as INPUT with internal pull-up resistor
//   initTLC(TLC_ADD, R_PIN);    // Initialise TLC LED Driver
//   setupLEDOutput(2);          // Set the mode of the LED Driver
//   initNAU7802();              // Initialise the ADC
//   Serial.println(Wire.getClock());
//   delay(3000); // Allow time for the initNAU to work,

//   // double referenceLightLevel = nau.read();
//   // for (int LED = 0; LED < 8; LED++)
//   // {
//   //   setBrightness(LED, 0xff); // Turn LED on
//   //   delay(100);
//   //   double reading = nau.read() - referenceLightLevel; // getCurrentReading(3); // Get ADC value
//   //   maxValues[LED] = reading;
//   //   String message = "LED" + String(LED) + " Max Value: " + String(reading); // Print LED No. and ADC value
//   //   Serial.println(message);
//   //   setBrightness(LED, 0x00); // Turn LED off
//   //   delay(100);
//   // }

//   // for (int LED = 0; LED < 8; LED++)
//   // {
//   //   double referenceLightLevel = getCurrentReading(10);
//   //   // Serial.println(referenceLightLevel);
//   //   setBrightness(LED, 0xff); // Turn LED on
//   //   delay(100);
//   //   double reading = getCurrentReading(10) - referenceLightLevel; // getCurrentReading(3); // Get ADC value
//   //   maxValues[LED] = reading;
//   //   String message = "LED" + String(LED) + " Max Value: " + String(reading); // Print LED No. and ADC value
//   //   Serial.println(message);
//   //   setBrightness(LED, 0x00); // Turn LED off
//   //   delay(100);
//   // }

//   Serial.println("Launch Ambient Lighting");
//   Serial.println(getCurrentReading(10));
// }

// void loop()
// {
//   buttonState = digitalRead(BUTTON_PIN);

//   // if (buttonState == HIGH)
//   // {
//   //   Serial.println(1);
//   //   delay(500);
//   //   int reference = nau.read();
//   //   Serial.println(reference);
//   //   setBrightness(0, 0xff);
//   //   delay(1000);
//   //   Serial.println(2);
//   //   Serial.println(nau.read()-reference);
//   //   // delay(10);
//   //   setBrightness(0, 0x00);
//   //   // delay(3000);
//   // }

//   if (buttonState == HIGH)
//   {
//     delay(1000);
//     // scanAll();
//     // flushReadings();
//     for (int LED = 0; LED < 8; LED++)
//     {
//       // double referenceLightLevel = nau.read();
//       setBrightness(LED, 0xff);
//       delay(500);
//       flushReadings(5);
//       // int absoluteLightLevel = (nau.read());
//       Serial.print(nau.read());

//       Serial.print(",");
//       setBrightness(LED, 0x00);
//       delay(500);
//     }
//     Serial.println();
//   }

//   // while (!nau.available())
//   // {
//   //   delay(1);
//   // }

//   // Serial.println(nau.read());
// }

// /* Test everything together **************************************/
// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_NAU7802.h>
// #include "TLCtest.h"

// #include <TFT_eSPI.h>
// #include <CST816S.h>
// #include <SPI.h>

// #define TLC_ADD 0x40 // I2C address of the TLC59108
// #define BUTTON_PIN 14
// #define R_PIN 27 // Uno: Pin2 ESP32: Pin32
// double maxValues[8];
// bool buttonState = 0;
// int lowest = 0;
// int highest = 0;

// Adafruit_NAU7802 nau;

// void initNAU7802()
// {
//   if (!nau.begin())
//   {
//     Serial.println("Failed to find NAU7802");
//   }
//   Serial.println("Found NAU7802");

//   nau.setGain(NAU7802_GAIN_8);
//   Serial.print("Gain set to 1x");

//   // Take 10 readings to flush out readings
//   for (uint8_t i = 0; i < 10; i++)
//   {
//     while (!nau.available())
//       delay(1);
//     nau.read();
//   }

//   nau.setRate(NAU7802_RATE_80SPS);

//   nau.setLDO(NAU7802_EXTERNAL);
//   // nau.setLDO(NAU7802_2V4);
//   Serial.println(nau.getLDO());
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
//   flushReadings(5);
//   double adcReadings = 0;

//   for (uint8_t i = 0; i < repeats; i++)
//   {
//     while (!nau.available())
//     {
//       delay(1);
//     }
//     adcReadings += nau.read();
//     // Serial.println(adcReadings);
//   }
//   adcReadings = adcReadings / repeats;

//   return adcReadings;
// }

// void scanAll()
// {
//   flushReadings(10);
//   for (int LED = 0; LED < 8; LED++)
//   {
//     double referenceLightLevel = getCurrentReading();
//     setBrightness(LED, 0xff);
//     delay(100);
//     int absoluteLightLevel = (getCurrentReading() - referenceLightLevel);
//     Serial.print(absoluteLightLevel);

//     Serial.print(",");
//     setBrightness(LED, 0x00);
//     delay(100);
//   }

//   // Get the reflectance values for combinations of the 8 LED's
//   for (int LED = 0; LED < 7; LED++)
//   {
//     double referenceLightLevel = getCurrentReading(3);
//     setBrightness(LED, 0x7F);
//     setBrightness(LED + 1, 0x7F);
//     delay(100);
//     int absoluteLightLevel = (getCurrentReading(3) - referenceLightLevel);
//     Serial.print(absoluteLightLevel);
//     if (LED < 6)
//     {
//       Serial.print(",");
//     }
//     setBrightness(LED, 0x00);
//     setBrightness(LED + 1, 0x00);
//     delay(100);
//   }
//   Serial.println();
// }

// void relScan()
// {
//   // Get first the main 8 readings
//   for (int LED = 0; LED < 8; LED++)
//   {
//     flushReadings(3);
//     while (!nau.available())
//       delay(1);
//     int relative = nau.read();
//     // Serial.println(relative);

//     ledOnOff(LED, 1);
//     while (!nau.available())
//       delay(1);
//     Serial.println((nau.read() - relative));

//     ledOnOff(LED, 0);
//   }

//   // Get the dual-lighting readings
//   for (int LED = 0; LED < 7; LED++)
//   {
//     flushReadings(3);
//     while (!nau.available())
//       delay(1);
//     int relative = nau.read();
//     // Serial.println(relative);

//     ledOnOff(LED, 1);
//     ledOnOff(LED+1, 1);
//     while (!nau.available())
//       delay(1);
//     Serial.println((nau.read() - relative));

//     ledOnOff(LED, 0);
//     ledOnOff(LED+1, 0);
//   }
// }

// void intensityOverTime()
// {
//   setupLEDOutput(2);
//   delay(50);

//   double referenceLightLevel = getCurrentReading(3);

//   for (int PWM = 0; PWM < 128; PWM++) // Set to 1/3 Brightness so it doesn't saturate
//   {
//     for (int LED = 0; LED < 8; LED++)
//     {
//       setBrightness(LED, PWM);
//     }
//     delay(10);
//     // Serial.println(getCurrentReading(3)-referenceLightLevel);
//     Serial.println(nau.read() - referenceLightLevel);
//     delay(10);
//   }

//   for (int PWM = 126; PWM >= 0; PWM--)
//   {
//     for (int LED = 0; LED < 8; LED++)
//     {
//       setBrightness(LED, PWM);
//     }
//     delay(10);
//     // Serial.println(getCurrentReading(3)-referenceLightLevel);
//     Serial.println(nau.read() - referenceLightLevel);
//     delay(10);
//   }
//   // initTLC(TLC_ADD, R_PIN); // Initialise TLC LED Driver
//   // setupLEDOutput(2);       // Set the mode of the LED Driver
//   delay(100);
// }

// void setup()
// {
//   Serial.begin(115200);
//   Wire.begin(); // Initialize I2C communication
//   Wire.setClock(1000000);
//   pinMode(BUTTON_PIN, INPUT); // Set the button pin as INPUT with internal pull-up resistor
//   initTLC(TLC_ADD, R_PIN);    // Initialise TLC LED Driver
//   // setupLEDOutput(2);          // Set the mode of the LED Driver
//   initNAU7802(); // Initialise the ADC
//   // Serial.println(Wire.getClock());
//   delay(1000); // Allow time for the initNAU to work,

//   Serial.println("Launch Ambient Lighting");
//   Serial.println(nau.read());

//   flushReadings(10);
// }

// void loop()
// {
  
//   while (!nau.available())
//       delay(1);
//     Serial.println((nau.read()));
//     delay(500);
//     ledOnOff(0, 1);
//     delay(100);
//     ledOnOff(0,0);
//     delay(100);
  
//   // buttonState = digitalRead(BUTTON_PIN);

//   // if (buttonState == HIGH)
//   // {
//   //   delay(1000);

//   // for (int LED = 0; LED < 8; LED++)
//   // {
//   //   flushReadings(5);
//   //   while (!nau.available())
//   //     delay(1);
//   //   int relative = nau.read();
//   //   // Serial.println(relative);

//   //   ledOnOff(LED, 1);
//   //   delay(10);
//   //   flushReadings(5);
//     // while (!nau.available())
//     //   delay(1);
//     // Serial.println((nau.read()));

//   //   ledOnOff(LED, 0);
//   //   delay(50);
//   // }

//   // }

//   // if (Serial.available() > 0)
//   // {
//   //   String input = Serial.readStringUntil('\n'); // Read input until newline character

//   //   // Convert input to lowercase for case-insensitive comparison
//   //   input.toLowerCase();

//   //   if (input == "s")
//   //   {
//   //     Serial.println("scan received");

//   //     int startTime = millis();
//   //     relScan();
//   //     int endTime = millis();

//   //     Serial.print("Execution time was: ");
//   //     Serial.println(endTime - startTime);
//   //   }
//   //   else if (input == "i")
//   //   {
//   //     Serial.println("intensity received");

//   //     intensityOverTime();

//   //   }
//   // }
// }

/*Display Code*/
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NAU7802.h>
#include "TLCtest.h"
#include <TFT_eSPI.h>
#include <CST816S.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

CST816S touch(21, 22, 12, 15); // sda, scl, rst, irq
String currentGesture;
bool beenHereBefore = 0;

Adafruit_NAU7802 nau;

void initNAU7802()
{
  if (!nau.begin(&Wire1))
  {
    Serial.println("Failed to find NAU7802");
  }
  Serial.println("Found NAU7802");

  nau.setGain(NAU7802_GAIN_8);
  Serial.print("Gain set to 1x");

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
  initTLC(0x40, 4);    // Initialise TLC LED Driver
  initNAU7802(); // Initialise the ADC
  delay(1000); // Allow time for the initNAU to work,

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

void startPage()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM); // Sets datum/origin of text to Middle-Centre of the screen
  tft.drawString("Ready to scan!", tft.width() / 2, tft.height() / 2, 4);
  tft.drawString("PLAS-ID", tft.width() / 2, 30, 2);
  tft.drawString("Hold down for 2 seconds", tft.width() / 2, (tft.height() / 2) + 30, 2);
  tft.drawString("to scan", tft.width() / 2, (tft.height() / 2) + 45, 2);
  tft.fillCircle(340,tft.height()/2, tft.width()/2, TFT_GREEN);
}

void setup()
{
  Serial.begin(115200);
  // Wire.begin();

  
  tft.init();
  startPage();

  touch.begin();

  Wire1.begin(32,33);
  initSenseModule();
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

    if (touch.gesture() == "SWIPE UP" && touch.gesture() != currentGesture)
    {
      printScreen(1);
      currentGesture = touch.gesture();
    }
    else if (touch.gesture() == "SWIPE RIGHT" && touch.gesture() != currentGesture)
    {
      printScreen(2);
      currentGesture = touch.gesture();
    }
    else if (touch.gesture() == "SWIPE DOWN" && touch.gesture() != currentGesture)
    {
      printScreen(3);
      currentGesture = touch.gesture();
    }
    else if (touch.gesture() == "SWIPE LEFT" && touch.gesture() != currentGesture)
    {
      printScreen(4);
      currentGesture = touch.gesture();
    }
    else if (touch.gesture() == "SINGLE CLICK" && touch.gesture() != currentGesture)
    {
      printScreen(5);
      currentGesture = touch.gesture();
    }
    else if (touch.gesture() == "LONG PRESS" && touch.gesture() != currentGesture)
    {
      printScreen(6);
      currentGesture = touch.gesture();
    }
  }


  // Serial.println(nau.read());
  ledOnOff(0,1);
  delay(500);
  ledOnOff(0,0);
  delay(500);
}