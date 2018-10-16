#include "display.h"
#include <string>
#include <exception>
#include "constdata.h"

bool operator ==(const Digit& left, const Digit& right)
{
    bool digitsEqual = true;
    for (unsigned int i = 0; i < g_linesInDigit; i++)
    {
        digitsEqual &= left.lines[i] == right.lines[i];
    }
    return digitsEqual;
}

unsigned int OCRLookupDigit(const Digit& digit)
{
    const auto foundDigit = std::find(s_digitsDict.begin(), s_digitsDict.end(), digit);
    if (foundDigit == s_digitsDict.end())
    {
        throw std::exception("Invalid digit.");
    }
    unsigned int value = std::distance(s_digitsDict.begin(), foundDigit);
    return value;
}

// Positions on display start from 0, from left to right
void OCRExtractDigitAtPos(const Display& display, unsigned int pos, Digit& target)
{
    for (unsigned int i = 0; i < g_linesInDigit; i++)
    {
        target.lines[i] = display.lines[i].substr(g_digitLen * pos, g_digitLen);
    }
}

unsigned int OCRScanDisplay(const Display& display)
{
    unsigned int value = 0;
    try
    {
        Digit displayDigit1;
        OCRExtractDigitAtPos(display, 0, displayDigit1);
        value = OCRLookupDigit(displayDigit1);

        Digit displayDigit2;
        OCRExtractDigitAtPos(display, 1, displayDigit2);
        if (!(displayDigit2 == s_emptyDigit))
        {
            value *= 10;
            value += OCRLookupDigit(displayDigit2);
        }
    }
    catch (const std::exception& /*ex*/)
    {
        //warning "invalid digit"
    }
    return value;
}
