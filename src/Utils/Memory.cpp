#include <Utils/Memory.hpp>
#include <CompilerLib/Lexer.hpp>
#include <Lyric/Tokenizer.hpp>

using namespace Utils;

Memory::Arena::Arena(size_t size){
    this->size = size;
    this->pointer = malloc(size);
    this->top = this->pointer;
};

template <class T>
T* Memory::Arena::Allocate(T value){
    size_t size = sizeof(value);
    if(((char*)top-(char*)pointer)+size>this->size){
        throw std::runtime_error("Run out of arena memory.");
    }
    T* ret = (T*)top;
    *ret = value;
    top=((char*)top+size);
    return (T*)ret;
}

using namespace CompilerLib::Lexer;
template std::string *Memory::Arena::Allocate<std::string>(std::string);
template std::wstring *Memory::Arena::Allocate<std::wstring>(std::wstring);
template Lyric::Token *Memory::Arena::Allocate<Lyric::Token>(Lyric::Token);
template int *Memory::Arena::Allocate<int>(int);