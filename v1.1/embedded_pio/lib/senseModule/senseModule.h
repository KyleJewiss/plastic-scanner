#ifndef senseModule_H
#define senseModule_H

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_NAU7802.h>
#include "TLCtest.h"

class senseModule
{
    public:

    senseModule(int sda, int scl);
    void begin();
    double senseModule::getCurrentReading(uint8_t repeats = 1);
    void senseModule::scanAll();
    


    private:
    int _sda, _scl;

    void initNAU7802();
    void flushReadings(uint8_t amount);

};

#endif