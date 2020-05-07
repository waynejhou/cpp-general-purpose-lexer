#ifndef LYRIC_TOKENIZER_HPP
#define LYRIC_TOKENIZER_HPP
#include <CompilerLib/WStringLexer.hpp>
#include <Utils/Memory.hpp>
#include <map>

namespace Lyric
{
#define TOKEN_TYPE_ENUMs \
    ENUM(None)           \
    ENUM(LeftBracket)    \
    ENUM(RightBracket)   \
    ENUM(Colon)          \
    ENUM(Dot)            \
    ENUM(Slash)          \
    ENUM(Equal)          \
    ENUM(EOL)            \
    ENUM(Number)         \
    ENUM(Value)          \
    ENUM(Lyric)          \
    ENUM(Identifier)

#define ENUM(a) a,
enum class TokenType
{
    TOKEN_TYPE_ENUMs
};
#undef ENUM

#define CAT(a, b) a##b
#define ENUM(a) CAT(L, #a),
const static std::wstring TokenTypeString[] = {
    TOKEN_TYPE_ENUMs};
#undef ENUM
#undef CAT

class Token
{
public:
    Token(std::wstring::iterator, std::wstring::iterator);
    TokenType type;
    int columnbegin;
    int columnend;
    std::string *file;
    int line;
    std::wstring str();
    std::wstring toString();

private:
    std::wstring::iterator head;
    std::wstring::iterator tail;
};

class Tokenizer : public CompilerLib::Lexer::WStringLexer
{
public:
    Tokenizer(): CompilerLib::Lexer::WStringLexer(){}
    std::vector<Token *> operator()(std::wstring *, Utils::Memory::Arena &);
    std::vector<Token *> operator()(std::wstring, Utils::Memory::Arena &);
};

} // namespace Lyric::Context

#endif