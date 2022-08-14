#ifndef __GY521_CONTROLLER__
#define __GY521_CONTROLLER__

#include <Arduino.h>
#include <Gy521.h>
#include <TimeParser.h>
#include <Settings.h>

#include <Axis.h>
#include <StepCounter.h>

class MotionDetector
{
private:
    MotionDetector(/* args */);
    MotionDetector(const MotionDetector &) = delete;
    MotionDetector operator=(const MotionDetector &) = delete;

    bool axisZdetect_ : 2;
    bool axisYdetect_ : 2;

    float currentTemp_ = 0;

    Gy521 &gy = Gy521::getInstance();

    Axis *axisZ_ = nullptr;
    Axis *axisY_ = nullptr;
    StepCounter *stepCounter_ = nullptr;

    static SemaphoreHandle_t axisMutex_;
    static SemaphoreHandle_t tempMutex_;
    static SemaphoreHandle_t stepsMutex_;

    TaskHandle_t gy521_task;

    uint16_t lastResetDay_ = 0;
    uint16_t lastYear_ = 0;

    uint16_t stepsOffset_ = 0;
    static uint32_t updateTimer_;

    static void updateCycle(void *args);

    static void checkForResetSteps();

public:
    static MotionDetector &getInstance();

    void initialize();
    void start();
    void stop();

    void saveSteps();

    bool isZDetectMotion();
    bool isYDetectMotion();

    uint16_t getStepsCount();
    uint16_t getStepsCountPerDay();

    float getTemperature();
};

extern MotionDetector &motionDetector;

#endif