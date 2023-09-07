#include "senseModule.h"

Adafruit_NAU7802 nau;

senseModule::senseModule(int sda, int scl)
{
    _sda = sda;
    _scl = scl;
}

void senseModule::begin()
{
    initTLC(0x40, 27); // Initialise TLC LED Driver
    initNAU7802(); // Initialise the ADC
    delay(1000); // Allow time for the initNAU to work,

    Serial.println("Launch Ambient Lighting");
    Serial.println(nau.read());
}

void senseModule::flushReadings(uint8_t amount)
{
  for (uint8_t i = 0; i < amount; i++)
  {
    while (!nau.available())
      delay(1);
    nau.read();
  }
}

double senseModule::getCurrentReading(uint8_t repeats = 1)
{
  flushReadings(5);
  double adcReadings = 0;

  for (uint8_t i = 0; i < repeats; i++)
  {
    while (!nau.available())
    {
      delay(1);
    }
    adcReadings += nau.read();
    // Serial.println(adcReadings);
  }
  adcReadings = adcReadings / repeats;

  return adcReadings;
}

void senseModule::scanAll()
{
  flushReadings(10);
  for (int LED = 0; LED < 8; LED++)
  {
    double referenceLightLevel = getCurrentReading();
    setBrightness(LED, 0xff);
    delay(100);
    int absoluteLightLevel = (getCurrentReading() - referenceLightLevel);
    Serial.print(absoluteLightLevel);

    Serial.print(",");
    setBrightness(LED, 0x00);
    delay(100);
  }

  // Get the reflectance values for combinations of the 8 LED's
  for (int LED = 0; LED < 7; LED++)
  {
    double referenceLightLevel = getCurrentReading(3);
    setBrightness(LED, 0x7F);
    setBrightness(LED + 1, 0x7F);
    delay(100);
    int absoluteLightLevel = (getCurrentReading(3) - referenceLightLevel);
    Serial.print(absoluteLightLevel);
    if (LED < 6)
    {
      Serial.print(",");
    }
    setBrightness(LED, 0x00);
    setBrightness(LED + 1, 0x00);
    delay(100);
  }
  Serial.println();
}

void initNAU7802()
{
    if (!nau.begin())
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