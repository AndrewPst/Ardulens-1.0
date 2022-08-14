#include <Axis.h>

#define ABS(__value__) ((__value__) < 0) ? ((__value__) * -1) : (__value__)

Axis::Axis(uint16_t treshold, uint8_t sensitivity)
{
    treshold_ = treshold;
    sensitivity_ = sensitivity;
    buffer = new int16_t[BUFFER_SIZE]{0};
}

bool Axis::DetectMotion(int16_t value)
{
    auto dif = calculateDifference(value);
    if (dif - avr_value > treshold_)
    {
        counter++;
        isMotion = counter >= sensitivity_;
    }
    else
    {
        isMotion = false;
        counter = 0;
        calculateAverage(dif);
    }
    return isMotion;
}

bool Axis::IsMotion()
{
    return isMotion;
}

uint16_t Axis::calculateDifference(int16_t &value)
{
    uint16_t difference = ABS(value - last_value);
    last_value = value;
    return difference;
}

void Axis::calculateAverage(uint16_t &dif)
{
    buffer[value_index] = dif;
    avr_value = 0;
    for (uint8_t i = 0; i < BUFFER_SIZE; i++)
    {
        avr_value += buffer[i];
    }
    avr_value = avr_value / BUFFER_SIZE;
    value_index = value_index >= BUFFER_SIZE ? 0 : value_index + 1;
}