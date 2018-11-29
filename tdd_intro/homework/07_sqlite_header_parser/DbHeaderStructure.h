#pragma once
#include <cstddef>
#include <cstdint>

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
    virtual void OpenDb() = 0;
    virtual std::size_t Read(std::size_t n, char* dest) = 0;
};

const std::size_t s_dbHeaderSize = 100;
const char s_dbDataHeader[]{"SQLite format 3"};

struct DbHeader {
    char dataHeader[sizeof(s_dbDataHeader)];
    uint16_t pageSize;
    uint8_t fileWriteFormatVersion;
    uint8_t fileReadFormatVersion;
    uint8_t payloadMaxFraction;
    uint8_t payloadMinFraction;
};

void DysplayHeaderStructure(IGui* gui, IDbReader* dbReader);
