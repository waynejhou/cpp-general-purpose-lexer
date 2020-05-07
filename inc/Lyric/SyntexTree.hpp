#ifndef LYRIC_SYNTEXTREE_HPP
#define LYRIC_SYNTEXTREE_HPP

#include <CompilerLib/LexerTemplate.hpp>
#include <Lyric/Tokenizer.hpp>
#include <vector>
#include <iostream>

namespace Lyric
{
using namespace CompilerLib::Lexer;
class SyntexTree : public LexerTemplate<std::vector<Lyric::Token *>::iterator, Lyric::TokenType>
{
public:
    void run(std::vector<Lyric::Token *>);
};
} // namespace Lyric

#endif