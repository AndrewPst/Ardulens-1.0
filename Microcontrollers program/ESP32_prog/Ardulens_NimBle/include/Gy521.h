#ifndef __GY521__
#define __GY521__

#include <Arduino.h>
#include <Wire.h>

#define MPU_ADDRESS 0x68

class Gy521
{
private:
    Gy521() = default;
    Gy521(const Gy521 &gy) = delete;
    Gy521 &operator=(const Gy521 &) = delete;

    TwoWire gyWire_ = TwoWire(2);

    SemaphoreHandle_t mpu_mutex;

public:
    static Gy521 &getInstance();
    void begin(int8_t sda, int8_t scl);
    void getAccelerometer(int16_t *accelX, int16_t *accelY, int16_t *accelZ);
    float getTemperature();
    void getGyro(int16_t *gyroX, int16_t *gyroY, int16_t *gyroZ);
};



#endif
