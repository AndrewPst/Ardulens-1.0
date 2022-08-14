#include <MotionDetector.h>

#define ENTER_AXIS xSemaphoreTake(axisMutex_, portMAX_DELAY)
#define EXIT_AXIS xSemaphoreGive(axisMutex_)

#define ENTER_TEMP xSemaphoreTake(tempMutex_, portMAX_DELAY)
#define EXIT_TEMP xSemaphoreGive(tempMutex_)

#define ENTER_STEPS xSemaphoreTake(stepsMutex_, portMAX_DELAY)
#define EXIT_STEPS xSemaphoreGive(stepsMutex_)

#define AUTOSAVE_STEPS_INTERVAL 3000

SemaphoreHandle_t MotionDetector::axisMutex_;
SemaphoreHandle_t MotionDetector::tempMutex_;
SemaphoreHandle_t MotionDetector::stepsMutex_;
uint32_t MotionDetector::updateTimer_ = 0;

MotionDetector::MotionDetector()
{
    axisMutex_ = xSemaphoreCreateMutex();
    tempMutex_ = xSemaphoreCreateMutex();
    stepsMutex_ = xSemaphoreCreateMutex();
}

MotionDetector &MotionDetector::getInstance()
{
    static MotionDetector detector;
    return detector;
}

void MotionDetector::initialize()
{
    //axisY_ = new Axis(5500, 3);
    //axisZ_ = new Axis(5500, 2);
    stepCounter_ = new StepCounter();

    lastResetDay_ = settings.getValue("l_day", 0);
    lastYear_ = settings.getValue("l_year", 1970);
    stepsOffset_ = settings.getValue("l_steps", 0);

    checkForResetSteps();

    xTaskCreate(updateCycle, "gy521", 4096, (void *)this, 1, &gy521_task);
}

void MotionDetector::start()
{
}

void MotionDetector::stop()
{
}

void MotionDetector::checkForResetSteps()
{
    auto timeData = timeNow.getTimeData();
    uint16_t year = atoi(timeData.getValue(T_YEAR).c_str());
    uint16_t day = atoi(timeData.getValue(T_DAY).c_str());

    ENTER_STEPS;
    if (year >= motionDetector.lastYear_)
    {
        if (day != motionDetector.lastResetDay_)
        {
            motionDetector.stepsOffset_ = 0;
            settings.setValue(year, "l_year");
            settings.setValue(day, "l_day");
            motionDetector.lastYear_ = year;
            motionDetector.lastResetDay_ = day;
        }
    }
    EXIT_STEPS;
}

void MotionDetector::saveSteps()
{
    ENTER_STEPS;
    settings.setValue((int32_t)(stepCounter_->getSteps() + motionDetector.stepsOffset_), "l_steps");
    EXIT_STEPS;
}

void MotionDetector::updateCycle(void *args)
{
    MotionDetector *md = (MotionDetector *)args;
    delay(100);
    uint32_t lastsavesteps = 0;
    md->gy.begin(SDA, SCL);
    for (;;)
    {
        int16_t accX, accY, accZ;
        md->gy.getAccelerometer(&accX, &accY, &accZ);
        md->stepCounter_->DetectSteps(accX, accY, accZ);

        // auto y = md->axisY_->DetectMotion(accY);
        // auto z = md->axisZ_->DetectMotion(accZ);
        // ENTER_AXIS;
        // md->axisYdetect_ = y;
        // md->axisZdetect_ = z;
        // EXIT_AXIS;

        ENTER_TEMP;
        md->currentTemp_ = md->gy.getTemperature();
        EXIT_TEMP;

        if (millis() - updateTimer_ > AUTOSAVE_STEPS_INTERVAL)
        {
            checkForResetSteps();
            if(lastsavesteps != md->stepCounter_->getSteps()){
                md->saveSteps();
                lastsavesteps = md->stepCounter_->getSteps();
            }
            md->saveSteps();
            updateTimer_ = millis();
        }

        delay(70);
    }
    vTaskDelete(nullptr);
}

bool MotionDetector::isYDetectMotion()
{
    ENTER_AXIS;
    auto value = axisYdetect_;
    EXIT_AXIS;
    return value;
}

bool MotionDetector::isZDetectMotion()
{
    ENTER_AXIS;
    auto value = axisZdetect_;
    EXIT_AXIS;
    return value;
}

uint16_t MotionDetector::getStepsCount()
{
    return stepCounter_->getSteps();
    //return 0;
}

uint16_t MotionDetector::getStepsCountPerDay()
{
    return getStepsCount() + stepsOffset_;
}

float MotionDetector::getTemperature()
{
    ENTER_TEMP;
    float result = currentTemp_;
    EXIT_TEMP;
    return result;
}

MotionDetector &motionDetector = MotionDetector::getInstance();