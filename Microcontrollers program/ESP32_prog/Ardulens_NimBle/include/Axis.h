
#ifndef __AXIS__
#define __AXIS__

#include <Arduino.h>

#define BUFFER_SIZE 30

class Axis
{
public:
    Axis(uint16_t treshold, uint8_t sensitivity);


    bool DetectMotion(int16_t value);
    bool IsMotion();

private:
    bool isMotion = false;

    int8_t counter = 0;
    uint16_t treshold_ = 0;
    uint8_t sensitivity_ = 0;
    int16_t *buffer;
    int8_t value_index = 0;
    int32_t avr_value = 0;
    int16_t last_value = 0;
    
    uint16_t calculateDifference(int16_t &value);
    void calculateAverage(uint16_t &dif);
};

#endif