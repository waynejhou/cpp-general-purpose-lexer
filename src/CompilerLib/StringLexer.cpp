#include <CompilerLib/StringLexer.hpp>

std::string CompilerLib::Lexer::StringLexer::cvt2String(char &chr)
{
    return std::string(1, chr);
}
char &CompilerLib::Lexer::StringLexer::charSelector(std::string::iterator &iter)
{
    return *iter;
}
int CompilerLib::Lexer::StringLexer::charComparer(char &a, char &b)
{
    return a - b;
}

CompilerLib::Lexer::StringLexer::StringLexer()
{
    Unit::setCvt2String(CompilerLib::Lexer::StringLexer::cvt2String);
    Unit::setComparer(CompilerLib::Lexer::StringLexer::charComparer);
    Unit::setSelector(CompilerLib::Lexer::StringLexer::charSelector);
}