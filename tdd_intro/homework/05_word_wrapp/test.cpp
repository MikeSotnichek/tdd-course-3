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
using Words = std::vector<std::string>;

std::string trim(const std::string& str)
{
    std::string trimmed = str;
    trimmed.erase(trimmed.cbegin(), std::find_if_not(trimmed.begin(), trimmed.end(), std::isspace));
    trimmed.erase(std::find_if_not(trimmed.rbegin(), trimmed.rend(), std::isspace).base(), trimmed.cend());
    return trimmed;
}

// Test list for SplitWords:
// single word
// single word with whitespace
// two words
// more whitespace between words
// more words
//

Words SplitWords(const std::string& str)
{
    Words words;
    auto pos = str.begin();
    while (pos != str.end())
    {
        auto end = std::find_if(pos, str.end(), std::isspace);
        std::string word = trim(std::string(pos, end));
        if (!word.empty())
        {
            words.push_back(word);
        }
        pos = std::find_if_not(end, str.end(), std::isspace);
    }
    return words;
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
    Words words = SplitWords(str);

    WrappedStrings strings;
    std::string wrap;
    for (const auto word : words)
    {
        if (!wrap.empty() && wrap.length() + word.length() > wrapLength)
        {
            strings.push_back(trim(wrap));
            wrap.clear();
        }

        if (word.length() > wrapLength)
        {
            size_t pos = 0;
            while (pos + wrapLength < word.length()) {
                wrap = word.substr(pos, wrapLength);
                strings.push_back(wrap);
                pos += wrapLength;
            }
            wrap = word.substr(pos, wrapLength);
        }
        else
        {
            wrap.append(word);
        }

        wrap.append(" ");
    }

    if (!wrap.empty())
    {
        strings.push_back(trim(wrap));
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

TEST(WrapString, LongStringWrapWordInMiddle)
{
    WrappedStrings shortStr = {"a", "bc"};
    EXPECT_EQ(shortStr, WrapString("a bc", 3));
}

TEST(WrapString, LongStringWrapWordInMiddle2)
{
    WrappedStrings shortStr = {"a", "bc", "d"};
    EXPECT_EQ(shortStr, WrapString("a bc d", 3));
}

TEST(WrapString, LongStringWrapWordAndSpaceEnd)
{
    WrappedStrings shortStr = {"a", "bcd", "e d"};
    EXPECT_EQ(shortStr, WrapString("a bcde d", 3));
}

TEST(WrapString, Acceptance)
{
    WrappedStrings shortStr = {
        "When pos is specified, the",
        "search only includes sequences",
        "of characters that begin at or",
        "before position pos, ignoring",
        "any possible match beginning",
        "after pos."
    };
    EXPECT_EQ(shortStr, WrapString("When pos is specified, the search  only includes sequences of characters that begin at or before position pos, ignoring any possible match beginning after pos.", 30));
}

TEST(WrapString, Acceptance2)
{
    WrappedStrings shortStr = {
        "When pos",
        "is",
        "specifie",
        "d, the",
        "search",
        "only",
        "includes",
        "sequence",
        "s of",
        "characte",
        "rs that",
        "begin at",
        "or",
        "before",
        "position",
        "pos,",
        "ignoring",
        "any",
        "possible",
        "match",
        "beginnin",
        "g after",
        "pos."
    };
    EXPECT_EQ(shortStr, WrapString("When pos is specified, the search  only includes sequences of characters that begin at or before position pos, ignoring any possible match beginning after pos.", 8));
}

TEST(SplitWords, OneWord)
{
    Words expected = {"a"};
    EXPECT_EQ(expected, SplitWords("a"));
}

TEST(SplitWords, OneWordWithSpace)
{
    Words expected = {"a"};
    EXPECT_EQ(expected, SplitWords(" a "));
}

TEST(SplitWords, TwoWords)
{
    Words expected = {"a", "b"};
    EXPECT_EQ(expected, SplitWords("a b"));
}

TEST(SplitWords, TwoWordsWithWhitespace)
{
    Words expected = {"a", "b"};
    EXPECT_EQ(expected, SplitWords(" a  b "));
}

TEST(SplitWords, MultipleWords)
{
    Words expected = {"ab", "cde", "fg"};
    EXPECT_EQ(expected, SplitWords("ab cde fg"));
}
