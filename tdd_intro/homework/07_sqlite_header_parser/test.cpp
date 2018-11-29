/*
Implement application for display sqlite header structure of given file.
Application takes path to file and displays header structure
Sqlite header is described here https://www.sqlite.org/fileformat.html
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class IGui
{
public:
    ~IGui(){}
    //Add here necessary methods
};

class IDbReader
{
public:
    ~IDbReader(){}
    //Add here necessary methods
};

// Mocks

class MockDbReader : IDbReader {

};

class MockGui : IGui {

};

void DysplayHeaderStructure(IGui* gui, IDbReader* dbReader)
{

}

/*
 * 1. Open db
 * 1.0. Reads 100 bytes
 * 1.1. Checks header string
 * 1.2. Throw if header not valid
 * 2. Parse structure
 * 2.1. Check page size power of two between 512 and 32768 inclusive, or the value 1 representing a page size of 65536
 * 2.2. Check file format version (1 or 2)
 * 2.3. More checks:
 * Maximum embedded payload fraction. Must be 64.
 * Minimum embedded payload fraction. Must be 32.
 * Leaf payload fraction. Must be 32.
 * The schema format number. Supported schema formats are 1, 2, 3, and 4.
 * The database text encoding. A value of 1 means UTF-8. A value of 2 means UTF-16le. A value of 3 means UTF-16be.
 * 3. Output to display
 * Prints values - a test for each of:
        The database page size in bytes.
        File format write version.
        File format read version.
        Bytes of unused "reserved" space at the end of each page. Usually 0.
        Maximum embedded payload fraction.
        Minimum embedded payload fraction.
        Leaf payload fraction.
        File change counter.
        Size of the database file in pages. (The "in-header database size")
        Page number of the first freelist trunk page.
        Total number of freelist pages.
        The schema cookie.
        The schema format number.
        Default page cache size.
        The page number of the largest root b-tree page when in auto-vacuum or incremental-vacuum modes, or zero otherwise.
        The database text encoding. UTF-8, UTF-16le or UTF-16be.
        The "user version" as read and set by the user_version pragma.
        incremental-vacuum mode (True or False).
        The "Application ID" set by PRAGMA application_id.
        Reserved for expansion. Must be zero.
        The version-valid-for number.
        SQLITE_VERSION_NUMBER

 * Improvements:
 *  1. Show warnings about errors
 *  2. check magical pragma values https://www.sqlite.org/src/artifact?ci=trunk&filename=magic.txt and display correct strings instead of raw values
 *  3. parse sqlite version number display as "<Major>.<Minor>.<Release>"
 */

TEST(DysplayHeaderStructure, OpensDb) {
    MockDbReader reader;
    MockGui gui;

    EXPECT_CALL(reader, OpenDb());

    DysplayHeaderStructure(&gui, &reader);
}
