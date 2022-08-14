#ifndef __TIMEPARSER__
#define __TIMEPARSER__

#include <Arduino.h>
#include <TimeData.h>

class TimeParser
{
public:

    String getDay();
    int getHours();
    int getMinutes();
    int getSeconds();
    TimeData_t getTimeData();

    void updateEpochTime();
    void setEpochTime(uint64_t time);
private:
    uint64_t epochTime_ = 0;
    uint32_t update_time = 0;
};

extern TimeParser timeNow;

#endif