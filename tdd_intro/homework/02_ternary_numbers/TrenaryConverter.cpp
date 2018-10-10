#include "TrenaryConverter.h"

unsigned int ConvertTrenaryDigit(const char trenaryDigit)
{
    switch (trenaryDigit)
    {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        default:
            return 0;
    }
}

unsigned int ConvertTrenary(const std::string& trenary)
{
    unsigned int pw = 1;
    unsigned int number = 0;
    for (auto it = trenary.rbegin(); it != trenary.rend(); ++it)
    {
        number += pw * ConvertTrenaryDigit(*it);
        pw *= 3;
    }

    return number;
}
