#include <Gy521.h>

#define ENTER_MPU xSemaphoreTake(mpu_mutex, portMAX_DELAY)
#define EXIT_MPU xSemaphoreGive(mpu_mutex)


void Gy521::begin(int8_t sda, int8_t scl)
{
    mpu_mutex = xSemaphoreCreateMutex();

    ENTER_MPU;
    gyWire_.begin(sda, scl);
    gyWire_.beginTransmission(MPU_ADDRESS);
    gyWire_.write(0x6B);       //  Power setting address
    gyWire_.write(0b00000000); // Disable sleep mode (just in case)
    gyWire_.endTransmission();
    gyWire_.beginTransmission(MPU_ADDRESS); //I2C address of the MPU
    gyWire_.write(0x1C);                    // Accelerometer config register
    gyWire_.write(0b00000000);              // 2g range +/- (default)
    gyWire_.endTransmission();
    EXIT_MPU;
}

void Gy521::getAccelerometer(int16_t *accelX, int16_t *accelY, int16_t *accelZ)
{
    ENTER_MPU;
    gyWire_.beginTransmission(MPU_ADDRESS); //I2C address of the MPU
    gyWire_.write(0x3B);                    //  Acceleration data register
    gyWire_.endTransmission();
    gyWire_.requestFrom(MPU_ADDRESS, 6); // Get 6 bytes, 2 for each DoF
    *accelX = gyWire_.read() << 8 | gyWire_.read();
    *accelY = gyWire_.read() << 8 | gyWire_.read();
    *accelZ = gyWire_.read() << 8 | gyWire_.read();
    EXIT_MPU;
}

float Gy521::getTemperature()
{
    ENTER_MPU;
    gyWire_.beginTransmission(MPU_ADDRESS); //I2C address of the MPU
    gyWire_.write(0x41);                    //  Acceleration data register
    gyWire_.endTransmission();
    gyWire_.requestFrom(MPU_ADDRESS, 2); // Get 6 bytes, 2 for each DoF
    int16_t value = gyWire_.read() << 8 | gyWire_.read();
    EXIT_MPU;
    float temp = value / 340.00f + 36.53f;
    return temp;
}

Gy521 &Gy521::getInstance()
{
    static Gy521 gy;
    return gy;
}