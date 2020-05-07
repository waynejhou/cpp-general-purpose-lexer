#include <iostream>
#include <Lyric/Tokenizer.hpp>
#include <Utils/Memory.hpp>
#include <iomanip>
#include <Macros.hpp>

using namespace Lyric;
using namespace Utils;

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::wcin.tie(0);
    std::wcin.imbue(std::locale("C.UTF-8"));
    std::wcout.imbue(std::locale("C.UTF-8"));
    std::wcerr.imbue(std::locale("C.UTF-8"));
    Memory::Arena arena(1024 * 1024);
    std::wstringstream input;
    std::wstring inputLine;
    while (getline(std::wcin, inputLine))
    {
        input << inputLine << "\n";
    }
    
    Tokenizer tokenizer;
    auto tokens = tokenizer(input.str(), arena);
    auto begin = tokens.begin();
    auto it = tokens.begin();
    auto end = tokens.end();
    while(it!=end){
        auto t = *it;
        if(it!=begin){
            if((*(it-1))->type==TokenType::EOL){
                std::wcout << ANSI_ESC_Reverse << std::setw(2) << t->line  << ANSI_ESC_Reset;
            }
        }else{
            std::wcout << ANSI_ESC_Reverse << std::setw(2) << t->line  << ANSI_ESC_Reset;
        }
        switch (t->type)
        {
        case TokenType::LeftBracket:
        case TokenType::RightBracket:
            std::wcout << ANSI_ESC_FG_LGRN ;
            break;
        case TokenType::Dot:
        case TokenType::Colon:
            std::wcout << ANSI_ESC_FG_LYEL ;
            break;
        case TokenType::Number:
            std::wcout << ANSI_ESC_FG_LBLU ;
            break;
        case TokenType::Identifier:
            std::wcout << ANSI_ESC_FG_BLU ;
            break;
        case TokenType::Value:
            std::wcout << ANSI_ESC_FG_MGT ;
            break;
        default:
            std::wcout << ANSI_ESC_Reset ;
            break;
        }
        std::wcout << t->str();
        it++;
    }
    return 0;
}
