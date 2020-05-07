#ifndef COMPILELIB_STRINGLEXER_HPP
#define COMPILELIB_STRINGLEXER_HPP

#include "./LexerTemplate.hpp"

namespace CompilerLib::Lexer
{
class StringLexer : public LexerTemplate<std::string::iterator, char>
{
protected:
    StringLexer();
    static std::string cvt2String(char& chr);
    static char& charSelector(std::string::iterator&);
    static int charComparer(char&, char&);
};
} // namespace CompilerLib::Lexer

#endif