#include "LCD.h"
#include <Arduino.h>

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
    tft.drawString("POLY-ID", tft.width() / 2, 30, 2);
    tft.drawString("Place against the plastic and", tft.width() / 2, (tft.height() / 2) + 30, 2);
    tft.drawString("press button to scan", tft.width() / 2, (tft.height() / 2) + 45, 2);
}

// void printPlasticType(String input_reading)
// {
//     tft.fillScreen(TFT_BLACK);
//     // tft.fillRect(0, 240, 160, 80, TFT_BLACK);
//     tft.setTextDatum(MC_DATUM); // Sets datum/origin of text to Middle-Centre of the screen
//     tft.drawString("ADC Reading:", tft.width() / 2, 50, 4);
//     tft.drawString(input_reading, tft.width() / 2, tft.height() / 2, 4);
// }

void loadingpage()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("Loading", tft.width() / 2, tft.height() / 2, 4);
}

void printPlasticType(String input_type, String input_likelihood)
{
    float likelihood_perc = input_likelihood.toInt();
    float theta =  360 * (((likelihood_perc-50)/100)*2);
    float thetaRad = 0;
    float radius = 95;
    int ringThickness = 10;

    float xCo = 0;
    float yCo = 0;
    float prexCo = 0;
    float preyCo = 0;

    int lightColour = 0;
    int darkColour = 0;

    if (input_type == "Unknown" || likelihood_perc < 60)
    {
        lightColour = 0xE986; // Light Red
        darkColour = 0xC904; // Dark Red
    }
    else if (likelihood_perc < 80)
    {
        lightColour = 0xFCC4; // Light Red
        darkColour = 0xDB21; // Dark Red
    }
    else
    {
        lightColour = MY_LGREEN; // Light Red
        darkColour = MY_DGREEN; // Dark Red
    }
    
    tft.fillScreen(lightColour);
    tft.setTextDatum(MC_DATUM); // Sets datum/origin of text to Middle-Centre of the screen
    tft.setTextColor(TFT_WHITE, lightColour);
    tft.drawString("Most Likely:", tft.width() / 2, tft.height() / 2 - 25, 2);
    tft.drawString(input_type, tft.width() / 2, tft.height() / 2, 4);
    tft.drawString((input_likelihood+="%"), 120, (120 + radius) - 30, 2);

    // Animate the loading bar
    for (int i = 0; i < theta; i ++)
    {
        thetaRad = i * (PI / 180);
        xCo = 120 - radius * sin(thetaRad);
        yCo = 120 + radius * cos(thetaRad);

        if (i > 0)
        {
            tft.fillCircle(prexCo, preyCo, ringThickness, darkColour);
        }

        tft.fillCircle(xCo, yCo, ringThickness, TFT_WHITE);

        if (i < 20)
        {
            tft.drawCircle(120, (120 + radius), ringThickness, TFT_WHITE);
        }

        prexCo = xCo;
        preyCo = yCo;

        delay(2);
    }
}

void printScreen(int input)
{
    tft.fillScreen(TFT_BLACK);
    tft.fillRect(80, 160, 160, 80, TFT_BLACK);
    tft.setTextDatum(MC_DATUM); // Sets datum/origin of text to Middle-Centre of the screen
    // tft.drawString("Plastic Type:", tft.width() / 2, 50, 4);
    tft.drawString(String(input), tft.width() / 2, tft.height() / 2, 6);
}

void failedReading()
{
    tft.fillScreen(TFT_RED);
    tft.setTextDatum(MC_DATUM); // Sets datum/origin of text to Middle-Centre of the screen
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Bad Reading", tft.width() / 2, 100, 4);
}
