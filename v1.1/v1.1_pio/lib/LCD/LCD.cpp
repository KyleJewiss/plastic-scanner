#include "LCD.h"

TFT_eSPI tft = TFT_eSPI();

void initLCD()
{
    tft.init();
}

void buzz(uint8_t buzz_length)
{
    digitalWrite(12, HIGH);
    delay(buzz_length);
    digitalWrite(12, LOW);
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
}

void printPlasticType(int input_reading)
{
    tft.fillRect(0, 240, 160, 80, TFT_BLACK);
    tft.setTextDatum(MC_DATUM); // Sets datum/origin of text to Middle-Centre of the screen
    tft.drawString("ADC Reading:", tft.width() / 2, 50, 4);
    tft.drawString(String(input_reading), tft.width() / 2, tft.height() / 2, 6);
}

void printScreen(int input)
{
    // tft.fillScreen(TFT_BLACK);
    tft.fillRect(80, 160, 160, 80, TFT_BLACK);
    tft.setTextDatum(MC_DATUM); // Sets datum/origin of text to Middle-Centre of the screen
    tft.drawString("Plastic Type:", tft.width() / 2, 50, 4);
    tft.drawString(String(input), tft.width() / 2, tft.height() / 2, 8);
}