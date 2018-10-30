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

// empty string
// string shorter than wrap number
// word longer than wrap number
// word much longer than wrap number (more than 2 strings)
// string longer than wrap number

// string wrapped by several whitespaces (less than wrapLength)
// string wrapped by several whitespaces (more than wrapLength)
// only whitespaces in string

using WrappedStrings = std::vector<std::string>;

WrappedStrings WrapString(const std::string& str, size_t wrapLength)
{
    return {};
}

TEST(WrapString, WrapEmptyString)
{
    WrappedStrings empty = {};
    EXPECT_EQ(empty, WrapString("", 1));
}
