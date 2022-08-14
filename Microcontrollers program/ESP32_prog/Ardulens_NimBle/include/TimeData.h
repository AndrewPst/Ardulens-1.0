#ifndef _TIMEDATA_T_
#define _TIMEDATA_T_

#include <Arduino.h>

enum Time_t : uint8_t
{
    T_HOUR = 0,
    T_MINUTE,
    T_SECOND,
    T_YEAR,
    T_MONTH,
    T_DAY,
    T_WEEKDAY
};

struct TimeData_t
{
    String getValue(Time_t time);

    String getDateStr(bool showYear = false);

    String getTimeStr(bool showSeconds = false);

    bool isEmpty();

    bool equal(const TimeData_t &data);
    
    bool operator==(const TimeData_t& data);

    bool operator!=(const TimeData_t& data);

    String date[7];
};
#endif