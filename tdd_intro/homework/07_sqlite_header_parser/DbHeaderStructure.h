#pragma once
#include <string>
#include <cstddef>
#include <cstdint>

class IGui
{
public:
    ~IGui(){}
    //Add here necessary methods
    virtual void Write(const std::string& message) = 0;
};

class IDbReader
{
public:
    ~IDbReader(){}
    //Add here necessary methods
    virtual void OpenDb() = 0;
    virtual std::size_t Read(std::size_t n, char* dest) = 0;
};

const std::size_t s_dbHeaderSize = 100;
const char s_dbDataHeader[]{"SQLite format 3"};
const unsigned int s_payloadMinFraction = 32;
const unsigned int s_payloadMaxFraction = 64;
const unsigned int s_payloadLeafFraction = 32;

enum class ReadWriteFormat : uint8_t {
    Undefined = 0,
    First = 1,
    Legacy = ReadWriteFormat::First,
    WAL,
    Last = ReadWriteFormat::WAL
};

enum class SchemaFormat : uint16_t {
    Undefined = 0,
    First = 1,
    FormatOne = SchemaFormat::First,
    FormatTwo,
    FormatThree,
    FormatFour,
    Last = SchemaFormat::FormatFour
};

enum class Encoding : uint16_t {
    Undefined = 0,
    First = 1,
    UTF8 = Encoding::First,
    UTF16Le,
    UTF16Be,
    Last = Encoding::UTF16Be
};

struct DbHeader {
    char dataHeader[sizeof(s_dbDataHeader)];
    uint16_t pageSize;
    ReadWriteFormat fileWriteFormatVersion;
    ReadWriteFormat fileReadFormatVersion;
    uint8_t payloadMaxFraction;
    uint8_t payloadMinFraction;
    uint8_t payloadLeafFraction;
    SchemaFormat schemaFormat;
    Encoding encoding;
};

void DisplayHeaderStructure(IGui* gui, IDbReader* dbReader);
