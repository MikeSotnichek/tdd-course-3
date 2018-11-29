#include "DbHeaderStructure.h"
#include <stdexcept>
#include <cassert>

namespace
{
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

        if(header->fileWriteFormatVersion != 1 && header->fileWriteFormatVersion != 2){
            throw std::runtime_error("Invalid file write format version.");
        }

        if(header->fileReadFormatVersion != 1 && header->fileReadFormatVersion != 2){
            throw std::runtime_error("Invalid file write format version.");
        }
    }
}

void DysplayHeaderStructure(IGui* gui, IDbReader* dbReader)
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
