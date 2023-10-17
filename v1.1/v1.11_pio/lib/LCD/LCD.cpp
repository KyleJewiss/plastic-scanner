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

void printPlasticType(String input_reading)
{
    float theta = 290;
    float thetaRad = 0;
    float radius = 95;
    int ringThickness = 10;

    float xCo = 0;
    float yCo = 0;
    float prexCo = 0;
    float preyCo = 0;
    
    tft.fillScreen(MY_LGREEN);
    tft.setTextDatum(MC_DATUM); // Sets datum/origin of text to Middle-Centre of the screen
    tft.setTextColor(TFT_WHITE, MY_LGREEN);
    tft.drawString("Most Likely:", tft.width() / 2, tft.height() / 2 - 25, 2);
    tft.drawString(input_reading, tft.width() / 2, tft.height() / 2, 4);
    tft.drawString("93%", 120, (120+radius)-30, 2);

    

    for (int i = 0; i < theta; i+=2)
    {
        thetaRad = i * (PI / 180);
        xCo = 120 -radius * sin(thetaRad);
        yCo = 120 +radius * cos(thetaRad);

        if (i > 0)
        {
            tft.fillCircle(prexCo, preyCo, ringThickness, MY_DGREEN);            
        }

        tft.fillCircle(xCo, yCo, ringThickness, TFT_WHITE);



        if (i < 20)
        {
            tft.drawCircle(120,(120+radius),ringThickness,TFT_WHITE);
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
