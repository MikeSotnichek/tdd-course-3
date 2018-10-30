/*
Write a function, that is given a string and a length limit, splits provided string into sequence of string,
where length of each string is not more, than provided limit. If there are spaces under provided limit -
last space should be used to wrapp this line. If there are no spaces - wrapp it on provided length limit.

Example:
When pos is specified, the search only includes sequences of characters that begin at or before position pos,
ignoring any possible match beginning after pos

"When pos is specified, the",
"search only includes sequences",
"of characters that begin at or",
"before position pos, ignoring",
"any possible match beginning",
"after pos."
*/

#include <gtest/gtest.h>
#include <cctype>

// empty string +
// string shorter than wrap number +
// word longer than wrap number +
// word much longer than wrap number (more than 2 strings) +
// string longer than wrap number +

// string wrapped in middle of word (should wrap on whitespace) +
// string wrapped in middle on two sides +
// wrap with word at end
// wrap by space and by word

// string wrapped by several whitespaces (less than wrapLength)
// string wrapped by several whitespaces (more than wrapLength)
// only whitespaces in string

using WrappedStrings = std::vector<std::string>;

WrappedStrings WrapString(const std::string& str, size_t wrapLength)
{
    WrappedStrings result;
    size_t pos = 0;
    while(pos < str.length())
    {
        std::string cur = str.substr(pos, wrapLength);

        auto lastWordStart = std::find_if(
                    cur.rbegin(),
                    cur.rend(),
                    std::isspace); // find the beginning of last word

        if (lastWordStart != cur.rend())
        {
            pos += cur.rend() - lastWordStart; // offset to include the last word in next wrap
            cur.erase((lastWordStart + 1).base(), cur.cend()); // erase the last word from current wrap
        }
        else
        {
            pos += wrapLength;
        }

        cur.erase(cur.cbegin(), std::find_if_not(cur.begin(), cur.end(), std::isspace)); // trim edges
        cur.erase(std::find_if_not(cur.rbegin(), cur.rend(), std::isspace).base(), cur.cend());

        if(!cur.empty())
        {
            result.push_back(cur);
        }
    }

    return result;
}

TEST(WrapString, EmptyString)
{
    ASSERT_EQ(WrappedStrings(), WrapString("", 25));
}

TEST(WrapString, StringShorterWrapNumber)
{
    ASSERT_EQ(WrappedStrings{"asdf"}, WrapString("asdf", 8));
}

TEST(WrapString, StringLongerThanWrapNumber)
{
    WrappedStrings expected = {"asd", "f"};
    ASSERT_EQ(expected, WrapString("asdf", 3));
}

TEST(WrapString, StringLongerThanWrapNumberSeveralParts)
{
    WrappedStrings expected = {"12", "34", "56"};
    ASSERT_EQ(expected, WrapString("123456", 2));
}

TEST(WrapString, MultipleWordsLonger)
{
    WrappedStrings expected = {"1", "2"};
    ASSERT_EQ(expected, WrapString("1 2", 1));
}

TEST(WrapString, SpaceStringEnd)
{
    WrappedStrings expected = {"1", "2"};
    ASSERT_EQ(expected, WrapString("1 2", 2));
}

TEST(WrapString, StringWrappedBySeveralWhitespace)
{
    WrappedStrings expected = {"12", "34"};
    ASSERT_EQ(expected, WrapString("12  34", 3));
}

TEST(WrapString, StringWrappedByALotOfWhitespace)
{
    WrappedStrings expected = {"12", "34"};
    ASSERT_EQ(expected, WrapString("12     34", 3));
}

TEST(WrapString, StringWrappedByALotOfWhitespaceOnTheLeft)
{
    WrappedStrings expected = {"123", "4"};
    ASSERT_EQ(expected, WrapString("123  4", 3));
}

TEST(WrapString, StringWrappedInMiddleOfWord)
{
    WrappedStrings expected = {"1234", "5678"};
    ASSERT_EQ(expected, WrapString("1234 5678", 4));
}

TEST(WrapString, StringWrappedInMiddleOfWord2)
{
    WrappedStrings expected = {"1", "23", "4"};
    ASSERT_EQ(expected, WrapString("1 23 4", 3));
}

TEST(WrapString, StringWrappedWordAtEnd)
{
    WrappedStrings expected = {"1 2", "3"};
    ASSERT_EQ(expected, WrapString("1 2 3", 3));
}
