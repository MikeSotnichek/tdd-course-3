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
#include "constdata.h"
#include "display.h"

/*
 * Test list:
 * A single digit 0 .. 9
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

unsigned int OCRScanDisplay(const Display& display)
{
    if(display.lines[0] == s_digit1.lines[0] && display.lines[1] == s_digit1.lines[1] && display.lines[2] == s_digit1.lines[2])
    {
        return 1;
    }
    if(display.lines[0] == s_digit2.lines[0] && display.lines[1] == s_digit2.lines[1] && display.lines[2] == s_digit2.lines[2])
    {
        return 2;
    }
    return 0;
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
