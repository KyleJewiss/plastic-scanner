#ifndef LCD_h
#define LCD_h

#include <Arduino.h>
#include <TFT_eSPI.h>

void initLCD();

void buzz(uint8_t buzz_length);

void printScreen(int input);

void startingDevicePage();

void startPage();

void printPlasticType(int input_reading);

#endif