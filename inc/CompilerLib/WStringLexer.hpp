#ifndef COMPILELIB_WSTRINGLEXER_HPP
#define COMPILELIB_WSTRINGLEXER_HPP

#include "./LexerTemplate.hpp"

namespace CompilerLib::Lexer
{
class WStringLexer : public LexerTemplate<std::wstring::iterator, wchar_t>
{
protected:
    WStringLexer();
    static std::wstring cvt2WString(wchar_t& chr);
    static wchar_t& wchar_tSelector(std::wstring::iterator&);
    static int wchar_tComparer(wchar_t&, wchar_t&);
};
} // namespace CompilerLib::Lexer

#endif