#include "DbHeaderStructure.h"
#include <stdexcept>
#include <cassert>
#include <iostream>

namespace
{
    void checkNumberValue(unsigned int number, unsigned int expected, const std::string& message)
    {
        if (number != expected)
        {
            throw std::runtime_error(message);
        }
    }

    template<typename ValueEnum>
    void checkValueInEnum(ValueEnum number, const std::string& message)
    {
        if (number < ValueEnum::First || number > ValueEnum::Last)
        {
            throw std::runtime_error(message);
        }
    }

    void checkValidHeader(DbHeader* header)
    {
        assert(header != nullptr);

        for (std::size_t i = 0; i < sizeof(s_dbDataHeader); ++i)
        {
            if (s_dbDataHeader[i] != header->dataHeader[i])
            {
                throw std::runtime_error("Invalid data header.");
            }
        }

        if (((header->pageSize & (header->pageSize - 1)) != 0 && header->pageSize != 1) || header->pageSize == 0)
        {
            throw std::runtime_error("Invalid page size.");
        }

        checkValueInEnum(header->fileWriteFormatVersion,
                         "Invalid file write format version.");

        checkValueInEnum(header->fileReadFormatVersion,
                         "Invalid file read format version.");

        checkNumberValue(header->payloadMinFraction,
                         s_payloadMinFraction,
                         "Invalid payloadMinFraction.");

        checkNumberValue(header->payloadMaxFraction,
                         s_payloadMaxFraction,
                         "Invalid payloadMaxFraction.");

        checkNumberValue(header->payloadLeafFraction,
                         s_payloadLeafFraction,
                         "Invalid payload leaf fraction.");

        checkValueInEnum(header->schemaFormat,
                         "Invalid schema format.");

        checkValueInEnum(header->encoding,
                         "Invalid encoding.");
    }
}

void DisplayHeaderStructure(IGui* gui, IDbReader* dbReader)
{
    if (dbReader == nullptr)
    {
        throw std::runtime_error("dbReader not initialized");
    }
    dbReader->OpenDb();
    char rawData[s_dbHeaderSize];
    const std::size_t readSize = dbReader->Read(s_dbHeaderSize, rawData);
    if (readSize < sizeof(DbHeader))
    {
        throw std::runtime_error("Cannot parse data header from file.");
    }
    DbHeader* parsed = reinterpret_cast<DbHeader*>(rawData);

    checkValidHeader(parsed);
}
