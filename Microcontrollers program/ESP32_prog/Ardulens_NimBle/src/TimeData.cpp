#include <TimeData.h>

String TimeData_t::getValue(Time_t time)
{
    return date[time];
}

String TimeData_t::getDateStr(bool showYear)
{
    return (showYear ? String(date[T_YEAR]) + '.' : "") + String(date[T_MONTH]) + '.' + String(date[T_DAY]);
}

String TimeData_t::getTimeStr(bool showSeconds)
{
    return String(date[T_HOUR]) + ':' + String(date[T_MINUTE]) + (showSeconds ? ':' + String(date[T_SECOND]) : "");
}

bool TimeData_t::isEmpty()
{
    for (uint8_t i = 0; i < 7; i++)
    {
        if (date[i].isEmpty())
            return true;
    }
    return false;
}

bool TimeData_t::equal(const TimeData_t &data)
{
    for (uint8_t i = 0; i < 7; i++)
    {
        if (!date[i].equals(data.date[i]) )
            return false;
    }
    return true;
}

bool TimeData_t::operator==(const TimeData_t &data)
{
    return equal(data);
}

bool TimeData_t::operator!=(const TimeData_t &data)
{
    return !equal(data);
}