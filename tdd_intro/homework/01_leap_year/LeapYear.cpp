#include "LeapYear.h"

bool IsLeapYear(int year)
{
    return year % 4 == 0 && year % 100 != 0;
}
