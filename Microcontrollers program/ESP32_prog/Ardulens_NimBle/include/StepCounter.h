#ifndef __STEPCOUNTER__
#define __STEPCOUNTER__

#include <Arduino.h>

#define MAX_STEPS_PAUSE_TIME 2000

#define MAX_STEPS_BUFFER_SIZE 10

#define BUFFER_SIZE_STEPS 10

class StepCounter
{
public:
    StepCounter();

    void DetectSteps(int16_t &x, int16_t &y, int16_t &z);

    uint16_t getSteps();
    void setSteps(uint16_t &count);

    ~StepCounter();

private:
    SemaphoreHandle_t mutex_;

    bool isChecked_;
    bool isWalk_;

    uint16_t *buffer_;

    uint8_t buffer_steps_counter_ = 0;
    uint8_t value_index_ = 0;

    uint16_t thresholdValue_;

    uint16_t steps_ = 0;
    uint32_t last_check_step_time_ = 0;
    int32_t avr_value_ = 0;
    int32_t last_value_ = 0;

    void CalculateAverage(int32_t &dif);

    int32_t CalculateDifference(int16_t &x, int16_t &y, int16_t &z);

    void RegisterStep();
};
#endif