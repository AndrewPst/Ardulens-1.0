#include <StepCounter.h>

#define THRESHOLD_VALUE_MIN 900
#define THRESHOLD_VALUE_MAX 1350

#define ABS(__value__) ((__value__) < 0) ? ((__value__) * -1) : (__value__)

StepCounter::StepCounter()
{
    mutex_ = xSemaphoreCreateMutex();
    buffer_ = new uint16_t[BUFFER_SIZE_STEPS]{5000};
    thresholdValue_ = THRESHOLD_VALUE_MAX;
}

void StepCounter::DetectSteps(int16_t &x, int16_t &y, int16_t &z)
{
    auto dif = CalculateDifference(x, y, z);
    if (dif - avr_value_ > thresholdValue_)
    {
        if (!isChecked_)
        {
            RegisterStep();
            isChecked_ = true;
        }
    }
    else
    {
        isChecked_ = false;
        CalculateAverage(dif);
    }
}

uint16_t StepCounter::getSteps()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);
    uint16_t result = steps_;
    xSemaphoreGive(mutex_);
    return result;
}

void StepCounter::setSteps(uint16_t &count)
{
    xSemaphoreTake(mutex_, portMAX_DELAY);
    steps_ = count;
    xSemaphoreGive(mutex_);
}

StepCounter::~StepCounter()
{
    delete[] buffer_;
}

void StepCounter::CalculateAverage(int32_t &dif)
{
    buffer_[value_index_] = dif;
    avr_value_ = 0;
    for (uint8_t i = 0; i < BUFFER_SIZE_STEPS; i++)
    {
        avr_value_ += buffer_[i];
    }
    avr_value_ = avr_value_ / BUFFER_SIZE_STEPS;
    value_index_ = value_index_ >= BUFFER_SIZE_STEPS ? 0 : value_index_ + 1;
}

int32_t StepCounter::CalculateDifference(int16_t &x, int16_t &y, int16_t &z)
{
    int32_t value = (x + y + z) / 3;
    uint32_t difference = ABS(value - last_value_);
    last_value_ = value;
    return difference;
}

void StepCounter::RegisterStep()
{
    if (millis() - last_check_step_time_ > MAX_STEPS_PAUSE_TIME)
    {
        buffer_steps_counter_ = 1;
        isWalk_ = false;
        thresholdValue_ = THRESHOLD_VALUE_MAX;
    }
    else
    {
        if (isWalk_)
        {
            xSemaphoreTake(mutex_, portMAX_DELAY);
            steps_++;
            xSemaphoreGive(mutex_);
        }
        else
        {
            if (buffer_steps_counter_ >= MAX_STEPS_BUFFER_SIZE)
            {
                isWalk_ = true;
                thresholdValue_ = THRESHOLD_VALUE_MIN;
                xSemaphoreTake(mutex_, portMAX_DELAY);
                steps_ += buffer_steps_counter_ + 1;
                xSemaphoreGive(mutex_);
            }
            else
            {
                buffer_steps_counter_++;
            }
        }
    }
    last_check_step_time_ = millis();
}