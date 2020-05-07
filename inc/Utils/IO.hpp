#ifndef UTILS_IO_HPP
#define UTILS_IO_HPP

#include <string>
#include <fstream>
#include <streambuf>
#include <stdexcept>
#include <sstream>

namespace Utils::IO
{
enum class Encoding
{
    UTF16
};
enum class OpenMode
{
    OpenExisting
};

class TextInputFile
{
public:
    TextInputFile(std::string path);
    std::string AsString();
private:
    std::wifstream stream;
};

class TextInputFileOpenArguments
{
public:
    TextInputFile operator()();
    Encoding encoding;
    OpenMode mode;
    std::string path;
};
} // namespace CompilerLib::IO

#endif