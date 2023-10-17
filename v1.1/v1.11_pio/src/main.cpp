/*Display Code*/
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NAU7802.h>
#include <BluetoothSerial.h>
#include "TLC.h"
#include "LCD.h"
#include <CST816S.h>

CST816S touch(21, 22, 26, 15); // sda, scl, rst, irq
bool allow = 0;

Adafruit_NAU7802 nau;

BluetoothSerial SerialBT;
bool bluth_tog = 0;

long startT = 0;

uint64_t sensor_readings[15];

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
  for (int LED = 0; LED < 8; LED++)
  {
    int ref = getNAUReading();
    setBrightness(LED, 0xff);
    delay(15);
    int relReading = getNAUReading() - ref;
    Serial.print(relReading);
    Serial.print(",");
    setBrightness(LED, 0x00);
    delay(15);
    sensor_readings[LED] = relReading;
  }

  // Get the reflectance values for combinations of the 8 LED's
  for (int LED = 0; LED < 7; LED++)
  {
    int ref = getNAUReading();
    setBrightness(LED, 0x7f);
    setBrightness(LED + 1, 0x7f);
    delay(15);
    int relReading = getNAUReading() - ref;
    Serial.print(relReading);
    if (LED < 6)
    {
      Serial.print(",");
    }
    setBrightness(LED, 0x00);
    setBrightness(LED + 1, 0x00);
    delay(15);

    sensor_readings[LED + 8] = relReading;
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
  SerialBT.begin("POLY_ID"); // Bluetooth device name
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
      // failedReading();
      bluth_tog = 1;
      buzz(150);
      delay(50);
      buzz(150);
    }
    else if (touch.gesture() == "SWIPE DOWN")
    {
      bluth_tog = 0;
      buzz(150);
      delay(50);
      buzz(150);
    }
  }

  

  // Serial Method
  if (buttonState == HIGH)
  {
    // printPlasticType("HDPE");
    if (bluth_tog == 0)
    {
      int startTime = millis();
      scanWithAllLEDs();

      delay(500);
      if (Serial.available())
      {
        String receivedData = Serial.readString(); // Read the incoming byte
        printPlasticType(receivedData);
      }

      int finishTime = millis();
      // printScreen((finishTime - startTime));
    }

    else
    {
      // Bluetooth method
      if (buttonState == HIGH)
      {
        int startTime = millis();
        scanWithAllLEDs();

        // Send sensor readings over SerialBT
        for (int value = 0; value < 15; value++)
        {
          SerialBT.print(sensor_readings[value]);
          if (value < 14)
          {
            SerialBT.print(","); // Add a comma as a separator, except for the last value
          }
        }
        SerialBT.println();

        while (!SerialBT.available())
        {
          delay(1);
        }
        String receivedData = SerialBT.readString(); // Read the incoming byte
        printPlasticType(receivedData);

        int finishTime = millis();
        // printScreen((finishTime - startTime));
      }
    }
  }
}