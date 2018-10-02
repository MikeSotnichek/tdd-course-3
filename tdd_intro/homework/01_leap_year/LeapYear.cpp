#include "LeapYear.h"

bool IsLeapYear(int year)
{
    if (year % 100 == 0)
        return false;
    return year % 4 == 0;
}
