#include "./Lexer.hpp"

#ifndef COMPILELIB_LEXERTEMPLATE_HPP
#define COMPILELIB_LEXERTEMPLATE_HPP

namespace CompilerLib::Lexer
{
template<class TIterator, class TLexValue>
class LexerTemplate
{
protected:
    typedef Vertex<TIterator> Vertex;
    typedef OrNode<TIterator> Or;
    typedef NextNode<TIterator> Next;
    typedef UnitNode<TIterator, TLexValue> Node;
    typedef Unit<TIterator, TLexValue> Unit;
    typedef EqualUnit<TIterator, TLexValue> Eq;
    typedef RangeUnit<TIterator, TLexValue> Rng;
    typedef ExcludeUnit<TIterator, TLexValue> Excld;
    Vertex &MakeRng(TLexValue from, TLexValue to){
        Vertex *ret = new Node(new Rng(from, to));
        return *ret;
    }
    Vertex &MakeEq(TLexValue value){
        Vertex *ret = new Node(new Eq(value));
        return *ret;
    }
    Vertex &MakeExcld(std::initializer_list<TLexValue> values){
        Vertex *ret = new Node(new Excld(values));
        return *ret;
    }
};
} // namespace CompilerLib::Lexer

#endif