#include <Lyric/SyntexTree.hpp>
#include <Lyric/Tokenizer.hpp>

using namespace Lyric;

int comparer(TokenType a, TokenType b)
{
    return (int)a - (int)b;
}

TokenType &selector(std::vector<Lyric::Token *>::iterator &iter)
{
    return (*iter)->type;
}

void SyntexTree::run(std::vector<Lyric::Token *> tokens)
{
    Unit::setComparer(comparer);
    Unit::setSelector(selector);
    auto &timeTag                             //
        = MakeEq(TokenType::LeftBracket)      //
          >> MakeEq(TokenType::Number)        //
          >> MakeEq(TokenType::Colon)         //
          >> MakeEq(TokenType::Number)        //
          >> MakeEq(TokenType::Dot)           //
          >> MakeEq(TokenType::Number)        //
          >> MakeEq(TokenType::RightBracket); //
    auto it = tokens.begin();
    std::wcout << timeTag(it) << std::endl;
}