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

using WrappedStrings = std::vector<std::string>;

// Test list for SplitWords:
// single word
// single word with whitespace
// two words
// more whitespace between words
// more words
//

std::vector<std::string> SplitWords(const std::string& str)
{
    return { str };
}

// Test list for WrapString:
// empty string
// string shorter than wrap number
// string shorter that ends with space
// string shorter that begins with space (trim spaces)
//
// word longer than wrap number
// word much longer than wrap number (more than 2 strings)
// string longer than wrap number
//
// string wrapped in middle of word

// split string by words, add words as necessary or cut long words.

WrappedStrings WrapString(const std::string& str, size_t wrapLength)
{
    size_t pos = 0;

    WrappedStrings strings;
    while (pos < str.length()) {

        std::string wrap = str.substr(pos, wrapLength);
        wrap.erase(wrap.cbegin(), std::find_if_not(wrap.begin(), wrap.end(), std::isspace));
        wrap.erase(std::find_if_not(wrap.rbegin(), wrap.rend(), std::isspace).base(), wrap.cend());
        if (!wrap.empty())
        {
            strings.push_back(wrap);
        }
        pos += wrapLength;

    }
    return strings;
}

TEST(WrapString, WrapEmptyString)
{
    WrappedStrings empty = {};
    EXPECT_EQ(empty, WrapString("", 1));
}

TEST(WrapString, ShortStringNoSpace)
{
    WrappedStrings shortStr = {"a"};
    EXPECT_EQ(shortStr, WrapString("a", 2));
}

TEST(WrapString, ShortStringSpaceEnd)
{
    WrappedStrings shortStr = {"a"};
    EXPECT_EQ(shortStr, WrapString("a ", 1));
}

TEST(WrapString, ShortStringSpaceBegin)
{
    WrappedStrings shortStr = {"a"};
    EXPECT_EQ(shortStr, WrapString(" a", 1));
}

TEST(WrapString, LongStringSplitIn2)
{
    WrappedStrings shortStr = {"a", "b"};
    EXPECT_EQ(shortStr, WrapString("ab", 1));
}

TEST(WrapString, LongStringSplitInMany)
{
    WrappedStrings shortStr = {"a", "b", "c", "d", "e"};
    EXPECT_EQ(shortStr, WrapString("abcde", 1));
}

TEST(WrapString, ShortStringWithSpace)
{
    WrappedStrings shortStr = {"a b"};
    EXPECT_EQ(shortStr, WrapString("a b", 4));
}

TEST(WrapString, LongStringWithSpace)
{
    WrappedStrings shortStr = {"a", "b"};
    EXPECT_EQ(shortStr, WrapString("a b", 1));
}

TEST(WrapString, LongStringWithSpace2)
{
    WrappedStrings shortStr = {"a b", "c"};
    EXPECT_EQ(shortStr, WrapString("a b c", 3));
}

TEST(WrapString, LongStringWithSpace3)
{
    WrappedStrings shortStr = {"a b", "c d", "e"};
    EXPECT_EQ(shortStr, WrapString("a b c d e", 3));
}

TEST(SplitWords, OneWord)
{
    std::vector<std::string> expected = {"a"};
    EXPECT_EQ(expected, SplitWords("a"));
}
