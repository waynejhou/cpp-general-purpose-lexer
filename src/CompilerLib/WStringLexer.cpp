#include <CompilerLib/WStringLexer.hpp>

std::wstring CompilerLib::Lexer::WStringLexer::cvt2WString(wchar_t &chr)
{
    return std::wstring(1, chr);
}
wchar_t &CompilerLib::Lexer::WStringLexer::wchar_tSelector(std::wstring::iterator &iter)
{
    return *iter;
}
int CompilerLib::Lexer::WStringLexer::wchar_tComparer(wchar_t &a, wchar_t &b)
{
    return a - b;
}

CompilerLib::Lexer::WStringLexer::WStringLexer()
{
    Unit::setCvt2WString(CompilerLib::Lexer::WStringLexer::cvt2WString);
    Unit::setComparer(CompilerLib::Lexer::WStringLexer::wchar_tComparer);
    Unit::setSelector(CompilerLib::Lexer::WStringLexer::wchar_tSelector);
}