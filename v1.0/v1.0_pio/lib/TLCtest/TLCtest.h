#ifndef TLCtest_h
#define TLCtest_h

#include <Arduino.h>
#include <Wire.h>

uint8_t setRegister(const uint8_t reg, const uint8_t value);
uint8_t setupLEDOutput(uint8_t outputMode);
uint8_t setBrightness(const uint8_t pwmChannel, const uint8_t dutyCycle);
void resetTLC();
void initTLC(uint8_t address, uint8_t resetPin);

#endif