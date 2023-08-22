// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_NAU7802.h>
// #include "TLCtest.h"

// #define TLC_ADD 0x40 // I2C address of the TLC59108
// #define BUTTON_PIN 14
// #define R_PIN 32 // Uno: Pin2 ESP32: Pin32
// bool buttonState = 0;
// double minValue = 0;
// double maxValue[8] = {};

// Adafruit_NAU7802 nau;

// void initNAU7802()
// {
//     if (!nau.begin())
//     {
//         Serial.println("Failed to find NAU7802");
//     }
//     Serial.println("Found NAU7802");

//     nau.setGain(NAU7802_GAIN_1);
//     Serial.print("Gain set to 1x");

//     // Take 10 readings to flush out readings
//     for (uint8_t i = 0; i < 10; i++)
//     {
//         while (!nau.available())
//             delay(1);
//         nau.read();
//     }

//     nau.setRate(NAU7802_RATE_20SPS);

//     while (!nau.calibrate(NAU7802_CALMOD_INTERNAL))
//     {
//         Serial.println("Failed to calibrate internal offset, retrying!");
//         delay(1000);
//     }
//     Serial.println("Calibrated internal offset");

//     while (!nau.calibrate(NAU7802_CALMOD_OFFSET))
//     {
//         Serial.println("Failed to calibrate system offset, retrying!");
//         delay(1000);
//     }
//     Serial.println("Calibrated system offset");
// }

// double getCurrentReading()
// {
//     int32_t adcReadings = 0;
//     double avgAdcReadings = 0;

//     // Read and record next 3
//     for (uint8_t i = 0; i < 3; i++)
//     {
//         while (!nau.available())
//         {
//             delay(1);
//         }
//         adcReadings += nau.read();
//     }

//     avgAdcReadings = adcReadings / 3;

//     // Serial.println(avgAdcReadings);
//     return avgAdcReadings;
// }

// double mapToRange(double value, int LED) {
//     return (value - minValue) * (1 - 0) / (maxValue[LED] - minValue) + 0;
// }

// void setup()
// {
//     Serial.begin(115200);
//     Wire.begin();               // Initialize I2C communication
//     pinMode(BUTTON_PIN, INPUT); // Set the button pin as INPUT with internal pull-up resistor
//     initTLC(TLC_ADD, R_PIN);    // Initialise TLC LED Driver
//     setupLEDOutput(2);          // Set the mode of the LED Driver
//     initNAU7802();              // Initialise the ADC
// }

// void loop()
// {
//     Serial.print("Enter a command: ");
//     while (Serial.available() == 0)
//     {
//         // delay(1); // Wait for user input
//     }

//     // String command = Serial.readStringUntil('\n');
//     // command.trim();
//     String command = Serial.readString();

//     if (command == "setMin")
//     {
//         double reading = 0;
//         for (int i = 0; i < 10; i++)
//         {
//             reading += getCurrentReading();
//         }
//         minValue = reading/10;
//         Serial.println("Min Value: ");
//         Serial.print(minValue);

//     }
//     else if (command == "setMax")
//     {
//         for (int LED = 0; LED < 8; LED++)
//         {
//             setBrightness(LED, 0xff); // Turn LED on
//             delay(100);
//             maxValue[LED] = getCurrentReading();
//             Serial.println(maxValue[LED]);
//             setBrightness(LED, 0x00); // Turn LED off
//         }
//     }
//     else if (command == "scanAll")
//     {
//         for (int LED = 0; LED < 8; LED++)
//         {
//             setBrightness(LED, 0xff); // Turn LED on
//             delay(100);
//             double reading = getCurrentReading();                          // Get ADC value
//             String message = "LED" + String(LED) + ": " + String(reading); // Print LED No. and ADC value
//             Serial.println(message);
//             setBrightness(LED, 0x00); // Turn LED off
//         }
//     }
//     else if (command == "scanAllCal")
//     {
//         for (int LED = 0; LED < 8; LED++)
//         {
//             setBrightness(LED, 0xff); // Turn LED on
//             delay(100);
//             double reading = getCurrentReading();                          // Get ADC value
//             Serial.println(mapToRange(reading, LED));
//             setBrightness(LED, 0x00); // Turn LED off
//         }
//     }
// }

/* Test everything together **************************************/
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NAU7802.h>
#include "TLCtest.h"

#define TLC_ADD 0x40 // I2C address of the TLC59108
#define BUTTON_PIN 14
#define R_PIN 32 // Uno: Pin2 ESP32: Pin32
double maxValues[8] = {0, 0, 0, 0, 0, 0, 0, 0};
bool buttonState = 0;
int lowest = 0;
int highest = 0;

Adafruit_NAU7802 nau;

void initNAU7802()
{
  if (!nau.begin())
  {
    Serial.println("Failed to find NAU7802");
  }
  Serial.println("Found NAU7802");

  nau.setGain(NAU7802_GAIN_16);
  Serial.print("Gain set to 1x");

  // Take 10 readings to flush out readings
  for (uint8_t i = 0; i < 10; i++)
  {
    while (!nau.available())
      delay(1);
    nau.read();
  }

  nau.setRate(NAU7802_RATE_40SPS);

  while (!nau.calibrate(NAU7802_CALMOD_INTERNAL))
  {
    Serial.println("Failed to calibrate internal offset, retrying!");
    delay(1000);
  }
  Serial.println("Calibrated internal offset");

  while (!nau.calibrate(NAU7802_CALMOD_OFFSET))
  {
    Serial.println("Failed to calibrate system offset, retrying!");
    delay(1000);
  }
  Serial.println("Calibrated system offset");
}

double getCurrentReading(int repeats)
{
  int32_t adcReadings = 0;
  double avgAdcReadings = 0;

  // Read and record next 3
  for (uint8_t i = 0; i < repeats; i++)
  {
    while (!nau.available())
    {
      delay(1);
    }
    adcReadings += nau.read();
  }

  avgAdcReadings = adcReadings / repeats;

  // Serial.println(avgAdcReadings);
  return avgAdcReadings;
}

void scanAll()
{
  delay(1000);
  double referenceLightLevel = getCurrentReading(3);
  // Serial.println(referenceLightLevel);
  // Get the reflectance values for the 8 LED's
  for (int LED = 0; LED < 8; LED++)
  {
    setBrightness(LED, 0xff);
    delay(100);
    int absoluteLightLevel = (getCurrentReading(3) - referenceLightLevel); // / (1 << 23);
    Serial.print(absoluteLightLevel);

    Serial.print(",");
    setBrightness(LED, 0x00);
    delay(100);
  }

  // Get the reflectance values for combinations of the 8 LED's
  for (int LED = 0; LED < 7; LED++)
  {
    setBrightness(LED, 0x7F);
    setBrightness(LED + 1, 0x7F);
    delay(100);
    int absoluteLightLevel = (getCurrentReading(3) - referenceLightLevel); // / (1 << 23);
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
  initTLC(TLC_ADD, R_PIN); // Initialise TLC LED Driver
  setupLEDOutput(2);       // Set the mode of the LED Driver
  delay(100);
}

void intensityOverTime()
{
  double referenceLightLevel = getCurrentReading(3);

  for (int PWM = 0; PWM < 128; PWM++) // Set to 1/3 Brightness so it doesn't saturate
  {
    for (int LED = 0; LED < 8; LED++)
    {
      setBrightness(LED, PWM);
    }
    delay(10);
    // Serial.println(getCurrentReading(3)-referenceLightLevel);
    Serial.println(nau.read() - referenceLightLevel);
    delay(10);
  }

  for (int PWM = 126; PWM >= 0; PWM--)
  {
    for (int LED = 0; LED < 8; LED++)
    {
      setBrightness(LED, PWM);
    }
    delay(10);
    // Serial.println(getCurrentReading(3)-referenceLightLevel);
    Serial.println(nau.read() - referenceLightLevel);
    delay(10);
  }
  initTLC(TLC_ADD, R_PIN); // Initialise TLC LED Driver
  setupLEDOutput(2);       // Set the mode of the LED Driver
  delay(100);
}

void setup()
{
  Serial.begin(115200);
  Wire.begin(); // Initialize I2C communication
  Wire.setClock(400000);
  pinMode(BUTTON_PIN, INPUT); // Set the button pin as INPUT with internal pull-up resistor
  initTLC(TLC_ADD, R_PIN);    // Initialise TLC LED Driver
  setupLEDOutput(2);          // Set the mode of the LED Driver
  initNAU7802();              // Initialise the ADC
  Serial.println(Wire.getClock());
  delay(3000); // Allow time for the initNAU to work,

  // double referenceLightLevel = nau.read();
  // for (int LED = 0; LED < 8; LED++)
  // {
  //   setBrightness(LED, 0xff); // Turn LED on
  //   delay(100);
  //   double reading = nau.read() - referenceLightLevel; // getCurrentReading(3); // Get ADC value
  //   maxValues[LED] = reading;
  //   String message = "LED" + String(LED) + " Max Value: " + String(reading); // Print LED No. and ADC value
  //   Serial.println(message);
  //   setBrightness(LED, 0x00); // Turn LED off
  //   delay(100);
  // }
  double referenceLightLevel = getCurrentReading(3);
  for (int LED = 0; LED < 8; LED++)
  {
    setBrightness(LED, 0xff); // Turn LED on
    delay(500);
    double reading = getCurrentReading(3) - referenceLightLevel; // getCurrentReading(3); // Get ADC value
    maxValues[LED] = reading;
    String message = "LED" + String(LED) + " Max Value: " + String(reading); // Print LED No. and ADC value
    Serial.println(message);
    setBrightness(LED, 0x00); // Turn LED off
    delay(100);
  }
}

void loop()
{
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == HIGH)
  {
    scanAll();
    // intensityOverTime();
    // setBrightness(0,0xff);

  }
  
}
