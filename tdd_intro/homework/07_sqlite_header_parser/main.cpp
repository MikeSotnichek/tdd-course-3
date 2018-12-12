#include "DbHeaderStructure.h"
#include <fstream>
#include <iostream>


class DbReader : public IDbReader
{
    public:
    DbReader(const std::string& filename) : m_fileName(filename){}
    virtual ~DbReader() override {
        file.close();
    }

    virtual void OpenDb() override {
        file.open(m_fileName);
        if (!file.good()) {
            throw std::runtime_error("Cannot open file.");
        }
    }

    virtual std::size_t Read(std::size_t n, char* dest) override {
        if (!file.good()) {
            throw std::runtime_error("File is not open.");
        }

        return file.readsome(dest, n);
    }

private:
    const std::string m_fileName;
    std::fstream file;
};

class Gui : public IGui {
public:
    virtual void Write(const std::string& msg) override {
        std::cout << msg << "\n";
    }
};

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Invalid arguments. Please use " << argv[0] << " <filename>\n";
    }

    DbReader reader(argv[0]);
    Gui gui;

    try {
        DisplayHeaderStructure(&gui, &reader);
    } catch (const std::runtime_error& ex) {
        std::cout << ex.what() << "\n";
    }

    return 0;
}
