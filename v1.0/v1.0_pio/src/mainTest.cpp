// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_NAU7802.h>

// Adafruit_NAU7802 nau;

// void setup() {
//   Serial.begin(115200);
//   Serial.println("NAU7802");
//   if (! nau.begin()) {
//     Serial.println("Failed to find NAU7802");
//   }
//   Serial.println("Found NAU7802");

//   // nau.setLDO(NAU7802_3V0);
//   // Serial.print("LDO voltage set to ");
//   // switch (nau.getLDO()) {
//   //   case NAU7802_4V5:  Serial.println("4.5V"); break;
//   //   case NAU7802_4V2:  Serial.println("4.2V"); break;
//   //   case NAU7802_3V9:  Serial.println("3.9V"); break;
//   //   case NAU7802_3V6:  Serial.println("3.6V"); break;
//   //   case NAU7802_3V3:  Serial.println("3.3V"); break;
//   //   case NAU7802_3V0:  Serial.println("3.0V"); break;
//   //   case NAU7802_2V7:  Serial.println("2.7V"); break;
//   //   case NAU7802_2V4:  Serial.println("2.4V"); break;
//   //   case NAU7802_EXTERNAL:  Serial.println("External"); break;
//   // }

//   nau.setGain(NAU7802_GAIN_1);
//   Serial.print("Gain set to ");
//   switch (nau.getGain()) {
//     case NAU7802_GAIN_1:  Serial.println("1x"); break;
//     case NAU7802_GAIN_2:  Serial.println("2x"); break;
//     case NAU7802_GAIN_4:  Serial.println("4x"); break;
//     case NAU7802_GAIN_8:  Serial.println("8x"); break;
//     case NAU7802_GAIN_16:  Serial.println("16x"); break;
//     case NAU7802_GAIN_32:  Serial.println("32x"); break;
//     case NAU7802_GAIN_64:  Serial.println("64x"); break;
//     case NAU7802_GAIN_128:  Serial.println("128x"); break;
//   }

//   // nau.setRate(NAU7802_RATE_320SPS);
//   // Serial.print("Conversion rate set to ");
//   // switch (nau.getRate()) {
//   //   case NAU7802_RATE_10SPS:  Serial.println("10 SPS"); break;
//   //   case NAU7802_RATE_20SPS:  Serial.println("20 SPS"); break;
//   //   case NAU7802_RATE_40SPS:  Serial.println("40 SPS"); break;
//   //   case NAU7802_RATE_80SPS:  Serial.println("80 SPS"); break;
//   //   case NAU7802_RATE_320SPS:  Serial.println("320 SPS"); break;
//   // }

//   // Take 10 readings to flush out readings
//   for (uint8_t i=0; i<10; i++) {
//     while (! nau.available()) delay(1);
//     nau.read();
//   }

//   while (! nau.calibrate(NAU7802_CALMOD_INTERNAL)) {
//     Serial.println("Failed to calibrate internal offset, retrying!");
//     delay(1000);
//   }
//   Serial.println("Calibrated internal offset");

//   while (! nau.calibrate(NAU7802_CALMOD_OFFSET)) {
//     Serial.println("Failed to calibrate system offset, retrying!");
//     delay(1000);
//   }
//   Serial.println("Calibrated system offset");
// }

// void loop() {
//   while (! nau.available()) {
//     delay(100);
//   }
//   float val = nau.read();
//   Serial.print("Read "); Serial.println(val,4);
// }

// // Working Analogue Frontend and ADC ///////////////////////////////////////////////////////
// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_NAU7802.h>

// #define buttonPin 14
// bool buttonState = 0;

// Adafruit_NAU7802 nau;

// void NAU7802_Setup()
// {
//   if (!nau.begin())
//   {
//     Serial.println("Failed to find NAU7802");
//   }
//   Serial.println("Found NAU7802");

//   nau.setGain(NAU7802_GAIN_128);
//   Serial.print("Gain set to 1x");

//   // Take 10 readings to flush out readings
//   for (uint8_t i = 0; i < 10; i++)
//   {
//     while (!nau.available())
//       delay(1);
//     nau.read();
//   }

//   while (!nau.calibrate(NAU7802_CALMOD_INTERNAL))
//   {
//     Serial.println("Failed to calibrate internal offset, retrying!");
//     delay(1000);
//   }
//   Serial.println("Calibrated internal offset");

//   while (!nau.calibrate(NAU7802_CALMOD_OFFSET))
//   {
//     Serial.println("Failed to calibrate system offset, retrying!");
//     delay(1000);
//   }
//   Serial.println("Calibrated system offset");
// }

// double getCurrentReading()
// {
//   double adcReadings = 0;
//   double avgAdcReadings = 0;

//   for (uint8_t i = 0; i < 10; i++)
//   {
//     while (!nau.available())
//     {
//       delay(1);
//     }
//     adcReadings += nau.read();
//   }

//   avgAdcReadings = adcReadings / 10;

//   Serial.println(avgAdcReadings);
//   return avgAdcReadings;
// }

// void setup()
// {
//   Serial.begin(115200);

//   pinMode(buttonPin, INPUT); // Set the button pin as INPUT with internal pull-up resistor

//   NAU7802_Setup();
// }

// void loop()
// {
//   buttonState = digitalRead(buttonPin);

//   if (buttonState == HIGH)
//   {
//     getCurrentReading();
//   }
// }

// // Working TLC
// #include <Arduino.h>
// #include <Wire.h>

// #define TLC59108_ADDR 0x40  // I2C address of the TLC59108
// #define RESET_PIN 32 // Uno: Pin2 ESP32: Pin32

// uint8_t setRegister(const uint8_t reg, const uint8_t value)
// {
//    Wire.beginTransmission(TLC59108_ADDR);
//    Wire.write(reg);
//    Wire.write(value);
//    return Wire.endTransmission();
// }

// void resetTLC() {
//   pinMode(RESET_PIN, OUTPUT);
//   digitalWrite(RESET_PIN, LOW);
//   delay(1);
//   digitalWrite(RESET_PIN, HIGH);
//   delay(1);

//   setRegister(0x00, 0x01);
// }

// uint8_t setupLEDOutput(uint8_t outputMode) {
//   // if (outputMode & 0xfc)
//   //   return ERROR::EINVAL;

//   byte regValue = (outputMode << 6) | (outputMode << 4) | (outputMode << 2) | outputMode;

//   uint8_t retVal = setRegister(0x0C, regValue);
//   retVal &= setRegister(0x0D, regValue);
//   return retVal;
// }

// uint8_t setBrightness(const uint8_t pwmChannel, const uint8_t dutyCycle)
// {
//   //  if(pwmChannel > 7)
//   //    return ERROR::EINVAL;

//    return setRegister(pwmChannel + 2, dutyCycle);
// }

// void setup() {
//   Wire.begin();  // Initialize I2C communication
//   resetTLC();
//   setupLEDOutput(2);
// }

// void loop() {
//   setBrightness(0,0xff);
//   delay(2000);
//   setBrightness(0,0x00);
//   delay(2000);
// }

// // Working TLC Own library
// #include <Arduino.h>
// #include <Wire.h>
// #include "TLCtest.h"

// #define TLC_ADD 0x40  // I2C address of the TLC59108
// #define R_PIN 32 // Uno: Pin2 ESP32: Pin32

// void setup() {
//   Wire.begin();  // Initialize I2C communication
//   initTLC(TLC_ADD,R_PIN);
//   setupLEDOutput(2);
// }

// void loop() {
//   setBrightness(0,0xff);
//   delay(2000);
//   setBrightness(0,0x00);
//   delay(2000);
// }

// /* Test everything together **************************************/
// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_NAU7802.h>
// #include "TLCtest.h"

// #define TLC_ADD 0x40 // I2C address of the TLC59108
// #define BUTTON_PIN 14
// #define R_PIN 32 // Uno: Pin2 ESP32: Pin32
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

//   nau.setRate(NAU7802_RATE_20SPS);

//   while (!nau.calibrate(NAU7802_CALMOD_INTERNAL))
//   {
//     Serial.println("Failed to calibrate internal offset, retrying!");
//     delay(1000);
//   }
//   Serial.println("Calibrated internal offset");

//   while (!nau.calibrate(NAU7802_CALMOD_OFFSET))
//   {
//     Serial.println("Failed to calibrate system offset, retrying!");
//     delay(1000);
//   }
//   Serial.println("Calibrated system offset");
// }

// double getCurrentReading()
// {
//   int32_t adcReadings = 0;
//   double avgAdcReadings = 0;

//   // Read and record next 3
//   for (uint8_t i = 0; i < 3; i++)
//   {
//     while (!nau.available())
//     {
//       delay(1);
//     }
//     adcReadings += nau.read();
//   }

//   avgAdcReadings = adcReadings / 3;

//   // Serial.println(avgAdcReadings);
//   return avgAdcReadings;
// }

// void setup()
// {
//   Serial.begin(115200);
//   Wire.begin();               // Initialize I2C communication
//   pinMode(BUTTON_PIN, INPUT); // Set the button pin as INPUT with internal pull-up resistor
//   initTLC(TLC_ADD, R_PIN);    // Initialise TLC LED Driver
//   setupLEDOutput(2);          // Set the mode of the LED Driver
//   initNAU7802();              // Initialise the ADC
// }

// void loop()
// {
//   // buttonState = digitalRead(BUTTON_PIN);

//   // if (buttonState == HIGH)
//   // {
//   //   delay(500);
//   //   String csvMessage = "";

//   //   for (int LED = 0; LED < 8; LED++)
//   //   {
//   //     setBrightness(LED, 0xff);                                      // Turn LED on
//   //     delay(100);
//   //     double reading = getCurrentReading();                          // Get ADC value
//   //     String message = "LED" + String(LED) + ": " + String(reading); // Print LED No. and ADC value
//   //     Serial.println(message);
//   //     setBrightness(LED, 0x00); // Turn LED off
//   //   }
//   // }

//   for (int LED = 0; LED < 8; LED++)
//   {
//     setBrightness(LED, 0xff); // Turn LED on
//     delay(100);
//     double reading = getCurrentReading();                          // Get ADC value
//     String message = "LED" + String(LED) + ": " + String(reading); // Print LED No. and ADC value
//     Serial.println(message);
//     setBrightness(LED, 0x00); // Turn LED off
//   }

//   Serial.println("waiting");
//   delay(3000);

//   // setBrightness(0, 0xff);
//   // Serial.println(getCurrentReading());

  
// }



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