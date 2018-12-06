/*
Implement application for display sqlite header structure of given file.
Application takes path to file and displays header structure
Sqlite header is described here https://www.sqlite.org/fileformat.html
*/


/*
 * 1. Open db
 * 1.0. Reads 100 bytes
 * 1.1. Checks header string
 * 1.2. Throw if header not valid
 * 2. Parse structure (throw if any check fails)
 * 2.1. Check page size power of two between 512 and 32768 inclusive, or the value 1 representing a page size of 65536
 * 2.2. Check file format version (1 or 2)
 * 2.3. More checks:
 * Maximum embedded payload fraction. Must be 64.
 * Minimum embedded payload fraction. Must be 32.
 * Leaf payload fraction. Must be 32.
 * The schema format number. Supported schema formats are 1, 2, 3, and 4.
 *
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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <DbHeaderStructure.h>

using namespace testing;

// Mocks

class MockDbReader : public IDbReader {
public:
    MOCK_METHOD0(OpenDb, void());
    MOCK_METHOD2(Read, std::size_t(std::size_t, char*));
};

class MockGui : public IGui {

};

// Helper functions for tests

void PrepareValidExpectedHeader(DbHeader &expected){
    std::memcpy(expected.dataHeader, s_dbDataHeader, sizeof(s_dbDataHeader));
    expected.pageSize = 1024;
    expected.fileWriteFormatVersion = ReadWriteFormat::WAL;
    expected.fileReadFormatVersion = ReadWriteFormat::WAL;
    expected.payloadMaxFraction = 64;
    expected.payloadMinFraction = 32;
    expected.payloadLeafFraction = 32;
    expected.schemaFormat = SchemaFormat::FormatOne;
    expected.encoding = Encoding::UTF8;
}


void ExpectHeaderRead(MockDbReader& reader, DbHeader *expected){
    char* readerData = reinterpret_cast<char*>(expected);

    EXPECT_CALL(reader, Read(s_dbHeaderSize, _)).WillOnce(DoAll(SetArrayArgument<1>(readerData, readerData + sizeof(DbHeader)), Return(sizeof(DbHeader))));
}

TEST(DisplayHeaderStructure, OpensDb) {
    MockDbReader reader;
    MockGui gui;

    EXPECT_CALL(reader, OpenDb());

    EXPECT_THROW(DisplayHeaderStructure(&gui, &reader), std::runtime_error);
}

TEST(DisplayHeaderStructure, ReadsDataFromFile) {
    MockDbReader reader;
    MockGui gui;

    EXPECT_CALL(reader, Read(s_dbHeaderSize, _));

    EXPECT_THROW(DisplayHeaderStructure(&gui, &reader), std::runtime_error);
}

TEST(DisplayHeaderStructure, AcceptsValidHeaders) {
    MockDbReader reader;
    MockGui gui;

    DbHeader expected;
    PrepareValidExpectedHeader(expected);

    ExpectHeaderRead(reader, &expected);

    EXPECT_NO_THROW(DisplayHeaderStructure(&gui, &reader));
}

TEST(DisplayHeaderStructure, ChecksInvalidDataHeader) {
    MockDbReader reader;
    MockGui gui;

    DbHeader expected;
    PrepareValidExpectedHeader(expected);
    std::strcpy(expected.dataHeader, "Invalid header");

    ExpectHeaderRead(reader, &expected);

    EXPECT_THROW(DisplayHeaderStructure(&gui, &reader), std::runtime_error);
}

TEST(DisplayHeaderStructure, ChecksInvalidPageSize) {
    MockDbReader reader;
    MockGui gui;

    DbHeader expected;
    PrepareValidExpectedHeader(expected);
    expected.pageSize = 100;

    ExpectHeaderRead(reader, &expected);

    EXPECT_THROW(DisplayHeaderStructure(&gui, &reader), std::runtime_error);
}

TEST(DisplayHeaderStructure, ChecksInvalidPageSize0) {
    MockDbReader reader;
    MockGui gui;

    DbHeader expected;
    PrepareValidExpectedHeader(expected);
    expected.pageSize = 0;

    ExpectHeaderRead(reader, &expected);

    EXPECT_THROW(DisplayHeaderStructure(&gui, &reader), std::runtime_error);
}

TEST(DisplayHeaderStructure, ChecksInvalidFileWriteFormatVersion) {
    MockDbReader reader;
    MockGui gui;

    DbHeader expected;
    PrepareValidExpectedHeader(expected);
    expected.fileWriteFormatVersion = ReadWriteFormat(3);

    ExpectHeaderRead(reader, &expected);

    EXPECT_THROW(DisplayHeaderStructure(&gui, &reader), std::runtime_error);
}

TEST(DisplayHeaderStructure, ChecksInvalidFileReadFormatVersion) {
    MockDbReader reader;
    MockGui gui;

    DbHeader expected;
    PrepareValidExpectedHeader(expected);
    expected.fileReadFormatVersion = ReadWriteFormat(3);

    ExpectHeaderRead(reader, &expected);

    EXPECT_THROW(DisplayHeaderStructure(&gui, &reader), std::runtime_error);
}

TEST(DisplayHeaderStructure, ChecksInvalidPayloadMinFraction) {
    MockDbReader reader;
    MockGui gui;

    DbHeader expected;
    PrepareValidExpectedHeader(expected);
    expected.payloadMinFraction = 0;

    ExpectHeaderRead(reader, &expected);

    EXPECT_THROW(DisplayHeaderStructure(&gui, &reader), std::runtime_error);
}

TEST(DisplayHeaderStructure, ChecksInvalidPayloadMaxFraction) {
    MockDbReader reader;
    MockGui gui;

    DbHeader expected;
    PrepareValidExpectedHeader(expected);
    expected.payloadMaxFraction = 0;

    ExpectHeaderRead(reader, &expected);

    EXPECT_THROW(DisplayHeaderStructure(&gui, &reader), std::runtime_error);
}

TEST(DisplayHeaderStructure, ChecksInvalidPayloadLeafFraction) {
    MockDbReader reader;
    MockGui gui;

    DbHeader expected;
    PrepareValidExpectedHeader(expected);
    expected.payloadLeafFraction = 0;

    ExpectHeaderRead(reader, &expected);

    EXPECT_THROW(DisplayHeaderStructure(&gui, &reader), std::runtime_error);
}

TEST(DisplayHeaderStructure, ChecksInvalidSchemaFormat) {
    MockDbReader reader;
    MockGui gui;

    DbHeader expected;
    PrepareValidExpectedHeader(expected);
    expected.schemaFormat = SchemaFormat::Undefined;

    ExpectHeaderRead(reader, &expected);

    EXPECT_THROW(DisplayHeaderStructure(&gui, &reader), std::runtime_error);
}

TEST(DisplayHeaderStructure, ChecksInvalidEncoding) {
    MockDbReader reader;
    MockGui gui;

    DbHeader expected;
    PrepareValidExpectedHeader(expected);
    expected.encoding = Encoding::Undefined;

    ExpectHeaderRead(reader, &expected);

    EXPECT_THROW(DisplayHeaderStructure(&gui, &reader), std::runtime_error);
}
