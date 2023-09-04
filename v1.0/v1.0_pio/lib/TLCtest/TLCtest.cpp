// Testing TLC
#include "TLCtest.h"

uint8_t TLC59108_ADDR; // I2C address of the TLC59108
uint8_t RESET_PIN;     // Uno: Pin2 ESP32: Pin32

void initTLC(uint8_t address, uint8_t resetPin)
{
    TLC59108_ADDR = address;
    RESET_PIN = resetPin;
    resetTLC();
    setRegister(0x00, 0x01);
}

uint8_t setRegister(const uint8_t reg, const uint8_t value)
{
    Wire1.beginTransmission(TLC59108_ADDR);
    Wire1.write(reg);
    Wire1.write(value);
    return Wire1.endTransmission();
}

void resetTLC()
{
    pinMode(RESET_PIN, OUTPUT);
    digitalWrite(RESET_PIN, LOW);
    delay(10);
    digitalWrite(RESET_PIN, HIGH);
    delay(10);
}

uint8_t setupLEDOutput(uint8_t outputMode)
{
    byte regValue = (outputMode << 6) | (outputMode << 4) | (outputMode << 2) | outputMode;
    Serial.print("RegValue: ");
    Serial.println(regValue);
    uint8_t retVal = setRegister(0x0C, regValue);
    retVal &= setRegister(0x0D, regValue);
    return retVal;
}

uint8_t setBrightness(const uint8_t pwmChannel, const uint8_t dutyCycle)
{
    return setRegister(pwmChannel + 2, dutyCycle);
}

void ledOnOff(uint8_t LED, uint8_t onOff)
{
    
    if (LED < 4)
    {
        byte message = onOff << (LED*2);
        setRegister(0x0C, message);
    }
    else if (LED > 3)
    {
        LED-=4;
        byte message = onOff << (LED*2);
        setRegister(0x0D, message);
    }

    delay(50); //Allows the LED to reach full brightness
}
