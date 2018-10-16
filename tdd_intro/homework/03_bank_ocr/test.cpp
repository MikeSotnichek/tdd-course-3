/*### Bank OCR

Your manager has recently purchased a machine that assists in reading letters and faxes sent in by branch offices. The machine scans the paper documents, and produces a file with a number of entries. You will write a program to parse this file.

#### Specification
#### User Story 1

The following format is created by the machine:
```
    _  _     _  _  _  _  _
  | _| _||_||_ |_   ||_||_|
  ||_  _|  | _||_|  ||_| _|
```
Each entry is 3 lines long, and each line has 27 characters. 3 lines of each entry contain an account number written using pipes and underscores.

Each account number should have 9 digits, all of which should be in the range 0-9. A normal file contains around 500 entries.

Write a program that can take this file and parse it into actual account numbers.

Example input and output
```
 _  _  _  _  _  _  _  _  _
| || || || || || || || || |
|_||_||_||_||_||_||_||_||_|

=> 000000000

  |  |  |  |  |  |  |  |  |
  |  |  |  |  |  |  |  |  |

=> 111111111

 _  _  _  _  _  _  _  _  _
 _| _| _| _| _| _| _| _| _|
|_ |_ |_ |_ |_ |_ |_ |_ |_

=> 222222222

 _  _  _  _  _  _  _  _  _
 _| _| _| _| _| _| _| _| _|
 _| _| _| _| _| _| _| _| _|

=> 333333333

|_||_||_||_||_||_||_||_||_|
  |  |  |  |  |  |  |  |  |

=> 444444444

 _  _  _  _  _  _  _  _  _
|_ |_ |_ |_ |_ |_ |_ |_ |_
 _| _| _| _| _| _| _| _| _|

=> 555555555

 _  _  _  _  _  _  _  _  _
|_ |_ |_ |_ |_ |_ |_ |_ |_
|_||_||_||_||_||_||_||_||_|

=> 666666666

 _  _  _  _  _  _  _  _  _
  |  |  |  |  |  |  |  |  |
  |  |  |  |  |  |  |  |  |

=> 777777777

 _  _  _  _  _  _  _  _  _
|_||_||_||_||_||_||_||_||_|
|_||_||_||_||_||_||_||_||_|

=> 888888888

 _  _  _  _  _  _  _  _  _
|_||_||_||_||_||_||_||_||_|
 _| _| _| _| _| _| _| _| _|

=> 999999999

    _  _     _  _  _  _  _
  | _| _||_||_ |_   ||_||_|
  ||_  _|  | _||_|  ||_| _|

=> 123456789
```
*/
#include <gtest/gtest.h>
#include <string>
#include <exception>
#include "constdata.h"
#include "display.h"

/*
 * Test list:
 * A single digit 0 .. 9
 *
 * Two digits 10
 * Two digits 01
 *
 * All same digits 0
 * All same gigits 1
 * ...
 * All same digits 9
 *
 * Different digits:
 * - Two kinds of digits
 * - Several kinds of digits
 *
 * //Not in specification:
 * Malformed input:
 * - Empty
 * - Not correct amount of lines (less, more)
 * - Not correct amount of columns (less, more)
 * - Not correct digits
*/

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

TEST(OCRScanDisplay, EmptyDisplay)
{
    const Display empty = {"", "", ""};
    EXPECT_EQ(OCRScanDisplay(empty), 0);
}

TEST(OCRScanDisplay, SingleDigit1)
{
    const Display one = { s_digit1.lines[0], s_digit1.lines[1], s_digit1.lines[2] };
    EXPECT_EQ(OCRScanDisplay(one), 1);
}

TEST(OCRScanDisplay, SingleDigit2)
{
    const Display two = { s_digit2.lines[0], s_digit2.lines[1], s_digit2.lines[2] };
    EXPECT_EQ(OCRScanDisplay(two), 2);
}

TEST(OCRScanDisplay, SingleDigit5)
{
    const Display five = { s_digit5.lines[0], s_digit5.lines[1], s_digit5.lines[2] };
    EXPECT_EQ(OCRScanDisplay(five), 5);
}

TEST(OCRScanDisplay, TwoDigits10)
{
    const Display ten = { s_digit1.lines[0] + s_digit0.lines[0],
                          s_digit1.lines[1] + s_digit0.lines[1],
                          s_digit1.lines[2] + s_digit0.lines[2] };
    EXPECT_EQ(OCRScanDisplay(ten), 10);
}

TEST(OCRScanDisplay, TwoDigits01)
{
    const Display one = { s_digit0.lines[0] + s_digit1.lines[0],
                          s_digit0.lines[1] + s_digit1.lines[1],
                          s_digit0.lines[2] + s_digit1.lines[2] };
    EXPECT_EQ(OCRScanDisplay(one), 1);
}
