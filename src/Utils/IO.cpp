#include <Utils/IO.hpp>

using namespace Utils::IO;

TextInputFile::TextInputFile(std::string path)
{
    this->stream.open(path);
    if (!this->stream.is_open())
    {
        throw std::runtime_error("File open failed.");
    }
}

std::string TextInputFile::AsString()
{
    if (!this->stream.is_open())
    {
        throw std::runtime_error("File is closed.");
    }

    std::stringstream buf;
    buf << this->stream.rdbuf();
    return buf.str();
}

TextInputFile TextInputFileOpenArguments::operator()()
{
    return TextInputFile(this->path);
}

