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
public:
    MOCK_METHOD1(Write, void(const std::string&));
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


void ExpectHeaderRead(MockDbReader& reader, DbHeader *expected) {
    char* readerData = reinterpret_cast<char*>(expected);

    EXPECT_CALL(reader, Read(s_dbHeaderSize, _)).WillOnce(DoAll(SetArrayArgument<1>(readerData, readerData + sizeof(DbHeader)), Return(sizeof(DbHeader))));
}

void ExpectHeaderWrite(MockGui& gui,
                       DbHeader& expected,
                       const std::string& readWriteFormat = "WAL / WAL",
                       const std::string& schemaFormat = "1",
                       const std::string& encoding = "UTF8") {
    InSequence sequence;
    EXPECT_CALL(gui, Write("Page size: " + std::to_string(expected.pageSize)));
    EXPECT_CALL(gui, Write("Read/Write format: " + readWriteFormat));
    EXPECT_CALL(gui, Write("Reserved space: " + std::to_string(expected.reservedSpace)));
    EXPECT_CALL(gui, Write("Payload Max/Min/Leaf fraction: "
                           + std::to_string(expected.payloadMaxFraction)
                           + "/"
                           + std::to_string(expected.payloadMinFraction)
                           + "/"
                           + std::to_string(expected.payloadLeafFraction)));
    EXPECT_CALL(gui, Write("Change counter: " + std::to_string(expected.changeCounter)));
    EXPECT_CALL(gui, Write("Pages count: " + std::to_string(expected.sizePages)));
    EXPECT_CALL(gui, Write("Freelist page trunk: " + std::to_string(expected.freelistPageTrunkNumber)));
    EXPECT_CALL(gui, Write("Schema format: " + schemaFormat));
    EXPECT_CALL(gui, Write("Schema cookie: " + std::to_string(expected.schemaCookie)));
    EXPECT_CALL(gui, Write("Page cache size: " + std::to_string(expected.pageCacheSize)));
    EXPECT_CALL(gui, Write("bTree root page: " + std::to_string(expected.bTreeRootPageNumber)));
    EXPECT_CALL(gui, Write("Encoding: " + encoding));
    EXPECT_CALL(gui, Write("User version: " + std::to_string(expected.userVersion)));
    EXPECT_CALL(gui, Write("Vacuum mode: " + std::to_string(expected.vacuumMode)));
    EXPECT_CALL(gui, Write("Application id: " + std::to_string(expected.applicationId)));
    EXPECT_CALL(gui, Write("Version valid for: " + std::to_string(expected.versionValidForNumber)));
    EXPECT_CALL(gui, Write("Sqlite version number: " + std::to_string(expected.sqliteVersionNumber)));
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

TEST(DisplayHeaderStructure, AcceptsARealStructure) {
    MockDbReader reader;
    MockGui gui;

    char rawExpectedData[] {0x53, 0x51, 0x4C, 0x69, 0x74, 0x65, 0x20, 0x66, 0x6F, 0x72, 0x6D, 0x61, 0x74, 0x20, 0x33, 0x00, 0x10, 0x00, 0x01, 0x01, 0x00, 0x40, 0x20, 0x20, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x2E, 0x01, 0x5A};

    EXPECT_CALL(reader, Read(s_dbHeaderSize, _)).WillOnce(DoAll(SetArrayArgument<1>(rawExpectedData, rawExpectedData + sizeof(rawExpectedData)), Return(sizeof(rawExpectedData))));

    EXPECT_NO_THROW(DisplayHeaderStructure(&gui, &reader));
}

TEST(DisplayHeaderStructure, PrintsInfoAboutStructure) {
    MockDbReader reader;
    MockGui gui;

    DbHeader expected;
    PrepareValidExpectedHeader(expected);

    ExpectHeaderRead(reader, &expected);

    ExpectHeaderWrite(gui, expected);

    EXPECT_NO_THROW(DisplayHeaderStructure(&gui, &reader));
}

TEST(DisplayHeaderStructure, DsiplaysEnumsInInfo) {
    MockDbReader reader;
    MockGui gui;

    DbHeader expected;
    PrepareValidExpectedHeader(expected);
    expected.encoding = Encoding::UTF16Be;
    expected.schemaFormat = SchemaFormat::FormatFour;
    expected.fileWriteFormatVersion = ReadWriteFormat::Legacy;
    expected.fileReadFormatVersion = ReadWriteFormat::Legacy;

    ExpectHeaderRead(reader, &expected);

    ExpectHeaderWrite(gui, expected, "Legacy / Legacy", "4", "UTF16Be");

    EXPECT_NO_THROW(DisplayHeaderStructure(&gui, &reader));
}
