#ifndef LCD_h
#define LCD_h

#include <Arduino.h>
#include <TFT_eSPI.h>

void initLCD();

void buzz(uint8_t buzz_length);

void printScreen(int input);

void startingDevicePage();

void startPage();

void printPlasticType(String input_reading, String input_likelihood);
void printPlasticType2(String input_reading);

void failedReading();

void loadingpage();

#endif