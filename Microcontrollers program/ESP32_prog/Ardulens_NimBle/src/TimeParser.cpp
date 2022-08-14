#include <TimeParser.h>

#define LEAP_YEAR(Y)     ( (Y>0) && !(Y%4) && ( (Y%100) || !(Y%400) ) )

const static String weekday_[] PROGMEM{
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
};

void TimeParser::updateEpochTime(){
    epochTime_ += (millis() - update_time)/1000;
    update_time = millis();
}

String TimeParser::getDay()
{
    return weekday_[(((epochTime_ / 86400L) + 4) % 7)]; //0 is Sunday
}
int TimeParser::getHours()
{
    return ((epochTime_ % 86400L) / 3600);
}
int TimeParser::getMinutes()
{
    return ((epochTime_ % 3600) / 60);
}
int TimeParser::getSeconds()
{
    return (epochTime_ % 60);
}

TimeData_t TimeParser::getTimeData()
{
    TimeData_t resultdata_;
    uint32_t hours = (epochTime_ % 86400L) / 3600;
    resultdata_.date[T_HOUR] = hours < 10 ? "0" + String(hours) : String(hours);

    uint32_t minutes = (epochTime_ % 3600) / 60;
    resultdata_.date[T_MINUTE] = minutes < 10 ? "0" + String(minutes) : String(minutes);

    uint32_t seconds = epochTime_ % 60;
    resultdata_.date[T_SECOND] = seconds < 10 ? "0" + String(seconds) : String(seconds);

    uint32_t rawTime = epochTime_ / 86400L; // in days
    uint32_t days = 0, year = 1970;
    uint8_t month;

    static const uint8_t monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    while ((days += (LEAP_YEAR(year) ? 366 : 365)) <= rawTime)
        year++;
    rawTime -= days - (LEAP_YEAR(year) ? 366 : 365); // now it is days in this year, starting at 0
    days = 0;
    for (month = 0; month < 12; month++)
    {
        uint8_t monthLength;
        if (month == 1)
        { // february
            monthLength = LEAP_YEAR(year) ? 29 : 28;
        }
        else
        {
            monthLength = monthDays[month];
        }
        if (rawTime < monthLength)
            break;
        rawTime -= monthLength;
    }
    resultdata_.date[T_YEAR] = String(year);
    resultdata_.date[T_MONTH] = ++month < 10 ? "0" + String(month) : String(month);     // jan is month 1
    resultdata_.date[T_DAY] = ++rawTime < 10 ? "0" + String(rawTime) : String(rawTime); // day of month
    resultdata_.date[T_WEEKDAY] = getDay();

    return resultdata_;
}

void TimeParser::setEpochTime(uint64_t time){
    this->epochTime_ = time;
    update_time = millis();
}

TimeParser timeNow = TimeParser();