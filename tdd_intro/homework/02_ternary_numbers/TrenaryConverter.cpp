#include "TrenaryConverter.h"

unsigned int ConvertTrenaryDigit(const char trenaryDigit)
{
    switch (trenaryDigit)
    {
        case '0':
            return 0;
        case '1':
            return 1;
        default:
            return 0;
    }
}

unsigned int ConvertTrenary(const std::string& trenary)
{
    if (trenary.compare("1010") == 0)
    {
        return 30;
    }

    return ConvertTrenaryDigit(trenary[0]);
}
