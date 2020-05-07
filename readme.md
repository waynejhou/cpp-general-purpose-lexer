# general-purpose-lexer

# Introduce

Same as title, it is a general purpose lexer library.

# Usage

* Inherit `CompilerLib::Lexer::LexerTemplate` to start.

* You can inherit `CompilerLib::Lexer::StringLexer` or  `CompilerLib::Lexer::WStringLexer` if string is what you parse.

* Template class `TIterator`, `TLexValue` is the iterator and value type append on what your data type is.
    * if your data is `std::string`, then
        * `TIterator`: `std::string::iterator`
        * `TLexValue`: `char`
    * if your data is `std::wstring`, then
        * `TIterator`: `std::wstring::iterator`
        * `TLexValue`: `wchar_t`
    * if your data is `std::vector<mytype>`, then
        * `TIterator`: `std::vector<mytype>::iterator`
        * `TLexValue`: `mytype` or anytype you can get from  mytype by define `Unit::selector`

* Build Lexer by
    * `MakeEq` for compare equal.
    * `MakeRng` for compare in range.
    * `MakeExcld` for exclude values.

# Sample
```c++

#include <CompilerLib/LexerTemplate.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class Sample
{
public:
    int value;
};

class SampleLexer : public CompilerLib::Lexer::LexerTemplate<std::vector<Sample>::iterator, int>
{
private:
    typedef std::vector<Sample>::iterator SampleIter;
    static std::string cvt2string(int &value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
    static int& selector(SampleIter &iter)
    {
        return iter->value;
    }
    static int comparer(int &a, int &b)
    {
        return a - b;
    }

public:
    SampleLexer(){
        Unit::setComparer(comparer);
        Unit::setSelector(selector);
        Unit::setCvt2String(cvt2string);
    }
    void parse(std::vector<Sample> samples)
    {
        auto &one = MakeEq(1)[([&]() { std::cout << "got a one.\n"; })];
        auto &two = +MakeEq(2)[([&]() { std::cout << "got a one or more twos.\n"; })];
        auto &three = MakeEq(3)[2][([&]() { std::cout << "got two threes.\n"; })];
        auto &four = *MakeEq(4)[([&]() { std::cout << "got zero or more fours.\n"; })];
        auto &five = MakeEq(5)[{2, -1}][([&]() { std::cout << "got two or more fives.\n"; })];
        auto &six = MakeEq(6)[([&]() { std::cout << "got a six.\n"; })];
        auto &four_five_six = (four >> five >> six)[([&]() { std::cout << "got a four five six.\n"; })];
        auto &lexer                                         //
            = one || two || three || four_five_six; //
        std::cout << lexer.toString() << std::endl;
        auto iter = samples.begin();
        auto end = samples.end();
        while (iter!=end)
        {
            if(!lexer(iter)){
                std::cout << iter->value << std::endl;
                throw std::runtime_error("Failed");
            }
        }
        std::cout << "Completed\n";
    }
};

int main(){
    std::vector<Sample> samples;
    samples.push_back(Sample());
    samples[0].value = 1;
    samples.push_back(Sample());
    samples[1].value = 3;
    samples.push_back(Sample());
    samples[2].value = 3;
    samples.push_back(Sample());
    samples[3].value = 2;
    samples.push_back(Sample());
    samples[4].value = 2;
    samples.push_back(Sample());
    samples[5].value = 2;
    samples.push_back(Sample());
    samples[6].value = 5;
    samples.push_back(Sample());
    samples[7].value = 5;
    samples.push_back(Sample());
    samples[8].value = 5;
    samples.push_back(Sample());
    samples[9].value = 5;
    samples.push_back(Sample());
    samples[10].value = 6;
    for(auto&s : samples){
        std::cout << s.value << " ";
    }
    std::cout << "\n";
    SampleLexer sl;
    sl.parse(samples);
    return 0;
}

```

```
1 3 3 2 2 2 5 5 5 5 6 
(1`|2+`|3{2}`|4*`5{2,}`6``)
got a one.
got two threes.
got a one or more twos.
got zero or more fours.
got two or more fives.
got a six.
got a four five six.
Completed
```