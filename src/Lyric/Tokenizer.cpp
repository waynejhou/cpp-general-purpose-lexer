#include <Lyric/Tokenizer.hpp>
#include <CompilerLib/Lexer.hpp>
#include <Utils/Memory.hpp>

using namespace Lyric;
using namespace Utils;

std::map<wchar_t, const wchar_t *> escapeChrs //
    = {
        {L'\n', L"\\n"},
        {L'\\', L"\\\\"},
        {L'/', L"\\/"},
        {L'[', L"\\["},
        {L']', L"\\]"},
        {L'(', L"\\("},
        {L')', L"\\)"},
        {L' ', L"\\ "},
};
std::wstring chr2escStr(wchar_t &chr)
{
    const wchar_t *mapped = escapeChrs[chr];
    if (mapped)
    {
        return std::wstring(mapped);
    }
    return std::wstring(1, chr);
}

std::wstring str2escStr(std::wstring str)
{
    std::wstringstream ss;
    for (auto chr : str)
    {
        ss << chr2escStr(chr);
    }
    return ss.str();
}

Token::Token(std::wstring::iterator head, std::wstring::iterator tail)
{
    this->head = head;
    this->tail = tail;
}

std::wstring Token::toString()
{
    std::wstringstream wss;
    wss << "[";
    wss << TokenTypeString[(int)type];
    wss << ": \"";
    wss << str2escStr(this->str());
    wss << "\"]";

    return wss.str();
}

std::wstring Token::str()
{
    return std::wstring(this->head, this->tail);
}

std::vector<Token *> Tokenizer::operator()(std::wstring *filename, Memory::Arena &arena)
{
    std::vector<Token *> ret;
    return ret;
}

std::vector<Token *> Tokenizer::operator()(std::wstring text, Memory::Arena &arena)
{
    auto str = arena.Allocate(text);
    auto begin = str->begin();
    auto start = str->begin();
    auto current = str->begin();
    auto end = str->end();
    std::vector<Token *> ret;
    int line = 1;
    int column = 0;
    TokenType type = TokenType::None;
    bool skip = false;
    enum LexMode
    {
        TEXT,
        LTAG,
        RTAG,
        TIME,

    };
    LexMode mode = TEXT;
    Unit::setCvt2WString(chr2escStr);
    auto &alphabet = (MakeRng('a', 'z') || MakeRng('A', 'Z')); //
    auto &time_digit = MakeRng('0', '9');
    auto &ltag_digit = MakeRng('0', '9');
    auto &LBR = MakeEq('[');
    auto &excLBR = +MakeExcld({'[', '\n'});
    auto &excRBR = +MakeExcld({']', '\n'});
    auto &rtag_RBR = MakeEq(']');
    auto &time_RBR = MakeEq(']');
    auto &dash = MakeEq('-');
    auto &underScope = MakeEq('_');
    auto &newLine = MakeEq('\n');
    auto &colon = MakeEq(':');
    auto &dot = MakeEq('.');
    auto &slash = MakeEq('/');
    //
    auto &mode_TEXT                                   //
        = LBR[([&] {                                  //
              type = TokenType::LeftBracket;          //
              mode = LTAG;                            //
          })]                                         //
          || (newLine[([&] {                          //
                 type = TokenType::EOL;               //
                 line++;                              //
                 column = 0;                          //
             })])                                     //
          || excLBR[([&] {                            //
                 type = TokenType::Lyric;             //
                 mode = TEXT;                         //
             })]                                      //
        ;                                             //
    auto &mode_LTAG                                   //
        = +ltag_digit[([&] {                          //
              type = TokenType::Number;               //
              mode = TIME;                            //
          })]                                         //
          || slash[([&] {                             //
                 type = TokenType::Slash;             //
             })]                                      //
          || +(alphabet || dash || underScope)[([&] { //
                 type = TokenType::Identifier;        //
                 mode = RTAG;                         //
             })]                                      //
        ;                                             //
    auto &mode_RTAG                                   //
        = colon[([&] {                                //
              type = TokenType::Colon;                //
          })]                                         //
          || excRBR[([&] {                            //
                 type = TokenType::Value;             //
             })]                                      //
          || rtag_RBR[([&] {                          //
                 type = TokenType::RightBracket;      //
                 mode = TEXT;                         //
             })]                                      //
        ;                                             //
    auto &mode_TIME                                   //
        = colon[([&] {                                //
              type = TokenType::Colon;                //
          })]                                         //
          || dot[([&] {                               //
                 type = TokenType::Dot;               //
             })]                                      //
          || time_RBR[([&] {                          //
                 type = TokenType::RightBracket;      //
                 mode = TEXT;                         //
             })]                                      //
          || +time_digit[([&] {                       //
                 type = TokenType::Number;            //
             })]                                      //
        ;                                             //

    //wcerr << mode_TEXT.toString() << endl;
    //wcerr << mode_LTAG.toString() << endl;
    //wcerr << mode_RTAG.toString() << endl;
    //wcerr << mode_TIME.toString() << endl;

    while (current != end)
    {
        //wcerr << "\"" << wstring(current, end) << "\"" << endl;
        bool result = false;
        switch (mode)
        {
        case TEXT:
            //wcout << "Mode TEXT" << endl;
            result = mode_TEXT(current);
            break;
        case LTAG:
            //wcout << "Mode LTAG" << endl;
            result = mode_LTAG(current);
            break;
        case RTAG:
            //wcout << "Mode RTAG" << endl;
            result = mode_RTAG(current);
            break;
        case TIME:
            //wcout << "Mode TIME" << endl;
            result = mode_TIME(current);
            break;
        default:
            break;
        }
        if (!result)
        {
            throw std::runtime_error("Failed to lex input.");
        }
        column += (current - start);
        if (skip)
        {
            start = current;
            continue;
        }
        Token t(start, current);
        t.columnbegin = (start - begin) + 1;
        t.columnend = column;
        t.type = type;
        t.line = line;

        Token *tptr = arena.Allocate(t);
        //wcerr << "Catched: " << tptr->toString() << endl;
        ret.push_back(tptr);
        start = current;
        type = TokenType::None;
    }
    return ret;
}
