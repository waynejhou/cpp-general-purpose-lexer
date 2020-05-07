// Reverse Code Implementation of Lexer
// from answer of stackexchange question "writing-a-lexer-in-c" by DeadMG

// stackexchange question: https://softwareengineering.stackexchange.com/questions/127889/writing-a-lexer-in-c
// answer: https://softwareengineering.stackexchange.com/a/127891
// by jhouwayne@gmail.com

#ifndef COMPILELIB_LEXER_HPP
#define COMPILELIB_LEXER_HPP

#include <functional>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

namespace CompilerLib::Lexer
{
template <class TIterator, class TLexValue>
class Unit
{
public:
    bool operator()(TIterator &iter)
    {
        bool vaild = this->isVaild(selector(iter));
        if (vaild)
            iter++;
        return vaild;
    }
    virtual bool isVaild(TLexValue value)
    {
        throw std::runtime_error("This function \"Unit::isVaild\" should be overrided.");
    }
    static void setCvt2WString(std::function<std::wstring(TLexValue&)> func)
    {
        cvt2WString = func;
    }
    static void setCvt2String(std::function<std::string(TLexValue&)> func)
    {
        cvt2String = func;
    }
    static void setSelector(std::function<TLexValue &(TIterator &)> func)
    {
        selector = func;
    }
    static void setComparer(std::function<int(TLexValue&, TLexValue&)> func)
    {
        comparer = func;
    }
    virtual std::wstring toWString()
    {
        throw std::runtime_error("This function \"Unit::toWString\" should be overrided.");
    }
    virtual std::string toString()
    {
        throw std::runtime_error("This function \"Unit::toString\" should be overrided.");
    }
    virtual ~Unit()
    {
    }

protected:
    Unit()
    {
        if (!selector)
            throw std::runtime_error("Use Unit::setSelector() to define how to get value from iterator.");
        if (!comparer)
            throw std::runtime_error("Use Unit::setComparer() to define how to compare value.");
    }
    Unit(Unit &other)
    {
    }
    Unit(const Unit &other)
    {
    }
    Unit(volatile Unit &other)
    {
    }
    Unit(volatile const Unit &other)
    {
    }
    static std::function<std::wstring(TLexValue&)> cvt2WString;
    static std::function<std::string(TLexValue&)> cvt2String;
    static std::function<TLexValue &(TIterator &)> selector;
    static std::function<int(TLexValue&, TLexValue&)> comparer;
};

template <class TIterator, class TLexValue>
std::function<std::wstring(TLexValue&)> Unit<TIterator, TLexValue>::cvt2WString = nullptr;
template <class TIterator, class TLexValue>
std::function<std::string(TLexValue&)> Unit<TIterator, TLexValue>::cvt2String = nullptr;
template <class TIterator, class TLexValue>
std::function<TLexValue &(TIterator &)> Unit<TIterator, TLexValue>::selector = nullptr;
template <class TIterator, class TLexValue>
std::function<int(TLexValue&, TLexValue&)> Unit<TIterator, TLexValue>::comparer = nullptr;
template <class TIterator, class TLexValue>
class EqualUnit : public Unit<TIterator, TLexValue>
{
public:
    ~EqualUnit() final
    {
    }
    EqualUnit(TLexValue value)
    {
        this->val = value;
    }
    bool isVaild(TLexValue value) override
    {
        return this->comparer(val, value) == 0;
    }
    std::string toString() override
    {
        if (!this->cvt2String)
            throw std::runtime_error("Use Unit::setCvt2String() to define how to convert to string.");
        return this->cvt2String(this->val);
    }
    std::wstring toWString() override
    {
        if (!this->cvt2String)
            throw std::runtime_error("Use Unit::setCvt2WString() to define how to convert to std::wstring.");
        return this->cvt2WString(this->val);
    }

private:
    TLexValue val;
};

template <class TIterator, class TLexValue>
class RangeUnit : public Unit<TIterator, TLexValue>
{
public:
    ~RangeUnit() final
    {
    }
    RangeUnit(TLexValue from, TLexValue to)
    {
        this->from = from;
        this->to = to;
    }
    bool isVaild(TLexValue value) override
    {
        return this->comparer(value, from) >= 0 && this->comparer(value, to) <= 0;
    }
    std::wstring toWString() override
    {
        if (!this->cvt2String)
            throw std::runtime_error("Call Unit::setCvt2WString first");
        std::wstringstream ss;
        ss << "["                          //
           << this->cvt2WString(this->from) //
           << "-"                          //
           << this->cvt2WString(this->to)   //
           << "]";                         //
        return ss.str();
    }
    std::string toString() override
    {
        if (!this->cvt2String)
            throw std::runtime_error("Call Unit::setCvt2String first");
        std::stringstream ss;
        ss << "["                          //
           << this->cvt2String(this->from) //
           << "-"                          //
           << this->cvt2String(this->to)   //
           << "]";                         //
        return ss.str();
    }

private:
    TLexValue from;
    TLexValue to;
};

template <class TIterator, class TLexValue>
class ExcludeUnit : public Unit<TIterator, TLexValue>
{
public:
    ~ExcludeUnit() final
    {
    }
    ExcludeUnit(std::initializer_list<TLexValue> values)
    {
        for (auto &val : values)
        {
            this->values.push_back(val);
        }
    }
    bool isVaild(TLexValue value) override
    {
        bool vaild = true;
        for (auto &val : values)
        {
            vaild = this->comparer(val, value) != 0;
            if (!vaild)
                break;
        }
        return vaild;
    }
    std::wstring toWString() override
    {
        if (!this->cvt2String)
            throw std::runtime_error("Call Unit::setCvt2WString first");
        std::wstringstream ss;
        ss << "[^"; //
        for (auto &val : values)
        {
            ss << this->cvt2WString(val);
        }
        ss << "]"; //
        return ss.str();
    }
    std::string toString() override
    {
        if (!this->cvt2String)
            throw std::runtime_error("Call Unit::setCvt2String first");
        std::stringstream ss;
        ss << "[^"; //
        for (auto &val : values)
        {
            ss << this->cvt2String(val);
        }
        ss << "]"; //
        return ss.str();
    }
private:
    std::vector<TLexValue> values;
};

template <class TIterator>
class NextNode;
template <class TIterator>
class OrNode;
template <class TIterator>
class Vertex
{
private:
    typedef std::function<void()> Action;

public:
    virtual bool operator()(TIterator &iter)
    {
        throw std::runtime_error("This function \"Vertex::operator()\" should be overrided.");
    }
    virtual std::wstring toWString()
    {
        throw std::runtime_error("This function \"Vertex::toWString\" should be overrided.");
    }
    virtual std::string toString()
    {
        throw std::runtime_error("This function \"Vertex::toString\" should be overrided.");
    }
    virtual ~Vertex()
    {
    }
    virtual Vertex &operator||(Vertex &other)
    {
        throw std::runtime_error("This function \"Vertex::operator||\" should be overrided.");
    }
    virtual Vertex &operator>>(Vertex &other)
    {
        throw std::runtime_error("This function \"Vertex::operator>>\" should be overrided.");
    }
    Vertex &operator*()
    {
        this->repeatFrom = 0;
        this->repeatTo = -1;
        return *this;
    }
    Vertex &operator+()
    {
        this->repeatFrom = 1;
        this->repeatTo = -1;
        return *this;
    }
    Vertex &operator!()
    {
        this->repeatFrom = 0;
        this->repeatTo = 1;
        return *this;
    }
    Vertex &operator[](Action action)
    {
        this->action = action;
        return *this;
    }
    Vertex &operator[](int repeatTimes)
    {
        this->repeatFrom = repeatTimes;
        this->repeatTo = repeatTimes;
        return *this;
    }
    Vertex &operator[](std::initializer_list<int> repeatTimes)
    {
        if (repeatTimes.size() > 0)
        {
            this->repeatFrom = *(repeatTimes.begin());
        }
        if (repeatTimes.size() > 1)
        {
            this->repeatTo = *(repeatTimes.begin()+1);
        }
        return *this;
    }
    virtual std::vector<Vertex *> getOrIngredient()
    {
        throw std::runtime_error("This function \"PoolNode::getOrIngredient\" should be overrided.");
    }
    virtual std::vector<Vertex *> getNextIngredient()
    {
        throw std::runtime_error("This function \"PoolNode::getNextIngredient\" should be overrided.");
    }

protected:
    Vertex()
    {
    }
    Vertex(Vertex &other)
    {
    }
    Vertex(const Vertex &other)
    {
    }
    Vertex(volatile Vertex &other)
    {
    }
    Vertex(volatile const Vertex &other)
    {
    }
    std::wstring getRepeatWString()
    {
        std::wstringstream ss;
        if (repeatFrom == 1 && repeatTo == 1)
        {
        }
        else if (repeatFrom == 0 && repeatTo == 1)
        {
            ss << "?";
        }
        else if (repeatFrom == 0 && repeatTo == -1)
        {
            ss << "*";
        }
        else if (repeatFrom == 1 && repeatTo == -1)
        {
            ss << "+";
        }
        else if (repeatFrom == repeatTo)
        {
            ss << "{";
            ss << repeatFrom;
            ss << "}";
        }
        else
        {
            ss << "{";
            ss << repeatFrom;
            ss << ",";
            if (repeatTo != -1)
            {
                ss << repeatTo;
            }
            ss << "}";
        }
        if (this->action)
        {
            ss << "`";
        }
        return ss.str();
    }
    std::string getRepeatString()
    {
        std::stringstream ss;
        if (repeatFrom == 1 && repeatTo == 1)
        {
        }
        else if (repeatFrom == 0 && repeatTo == 1)
        {
            ss << "?";
        }
        else if (repeatFrom == 0 && repeatTo == -1)
        {
            ss << "*";
        }
        else if (repeatFrom == 1 && repeatTo == -1)
        {
            ss << "+";
        }
        else if (repeatFrom == repeatTo)
        {
            ss << "{";
            ss << repeatFrom;
            ss << "}";
        }
        else
        {
            ss << "{";
            ss << repeatFrom;
            ss << ",";
            if (repeatTo != -1)
            {
                ss << repeatTo;
            }
            ss << "}";
        }
        if (this->action)
        {
            ss << "`";
        }
        return ss.str();
    }
    bool isNeedPacked()
    {
        return this->action != nullptr                            //
               || (this->repeatFrom != 1 && this->repeatTo != 1); //
    };
    bool repeat(std::function<bool()> parse)
    {
        bool vaild = false;
        if (this->repeatFrom == 0)
        {
            vaild = true;
        }
        for (int i = 0; i < this->repeatFrom; i++)
        {

            vaild = parse();
            if (!vaild)
                break;
        }

        for (int i = this->repeatFrom; i != this->repeatTo; i++)
        {
            bool vaild = parse();
            if (!vaild)
                break;
        }
        return vaild;
    }
    Action action = nullptr;
    int repeatFrom = 1;
    int repeatTo = 1;
};

template <class TIterator>
class PoolNode : public Vertex<TIterator>
{
private:
    typedef Vertex<TIterator> Vertex;
    typedef std::vector<Vertex *> VertexList;

protected:
    std::vector<Vertex *> pool;
    PoolNode(std::initializer_list<Vertex *> vertices)
    {
        for (auto &v : vertices)
        {
            pool.push_back(v);
        }
    }
    PoolNode(Vertex *vertex, VertexList vertices)
    {
        pool.push_back(vertex);
        for (auto &v : vertices)
        {
            pool.push_back(v);
        }
    }
    PoolNode(VertexList vertices, Vertex *vertex)
    {
        for (auto &v : vertices)
        {
            pool.push_back(v);
        }
        pool.push_back(vertex);
    }
    PoolNode(VertexList verticesL, VertexList verticesR)
    {
        for (auto &v : verticesL)
        {
            pool.push_back(v);
        }
        for (auto &v : verticesR)
        {
            pool.push_back(v);
        }
    }
    PoolNode(PoolNode &other)
    {
    }
    PoolNode(const PoolNode &other)
    {
    }
    PoolNode(volatile PoolNode &other)
    {
    }
    PoolNode(volatile const PoolNode &other)
    {
    }
};

template <class TIterator>
class OrNode : public PoolNode<TIterator>
{
private:
    typedef Vertex<TIterator> Vertex;
    typedef PoolNode<TIterator> PoolNode;
    typedef NextNode<TIterator> Next;
    typedef OrNode<TIterator> Or;

public:
    ~OrNode() final
    {
    }
    OrNode(std::initializer_list<Vertex *> vertices)
        : PoolNode(vertices)
    {
    }
    OrNode(Vertex *vertex, std::vector<Vertex *> vertices)
        : PoolNode(vertex, vertices)
    {
    }
    OrNode(std::vector<Vertex *> vertices, Vertex *vertex)
        : PoolNode(vertices, vertex)
    {
    }
    OrNode(std::vector<Vertex *> verticesL, std::vector<Vertex *> verticesR)
        : PoolNode(verticesL, verticesR)
    {
    }
    std::vector<Vertex *> getOrIngredient() override
    {
        if (this->isNeedPacked())
        {
            std::vector<Vertex *> ret;
            ret.push_back(this);
            return ret;
        }
        else
        {
            return this->pool;
        }
    }
    std::vector<Vertex *> getNextIngredient() override
    {
        std::vector<Vertex *> ret;
        ret.push_back(this);
        return ret;
    }
    Vertex &operator||(Vertex &other) override
    {
        if (this->isNeedPacked())
        {
            Vertex *ret = new OrNode(this, other.getOrIngredient());
            return *ret;
        }
        Vertex *ret = new OrNode(this->pool, other.getOrIngredient());
        return *ret;
    }
    Vertex &operator>>(Vertex &other) override
    {
        Vertex *ret = new Next(this->pool, other.getNextIngredient());
        return *ret;
    }
    bool operator()(TIterator &iter) override
    {
        auto orFunc = [&]() {
            bool vaild = false;
            for (auto &v : this->pool)
            {
                vaild = (*v)(iter);
                if (vaild)
                    break;
            }
            return vaild;
        };
        bool vaild = this->repeat(orFunc);
        if (vaild && this->action)
            this->action();
        return vaild;
    }
    std::string toString() override
    {
        std::stringstream ss;
        ss << "(";
        ss << (*(this->pool[0])).toString();
        for (int i = 1; i < this->pool.size(); i++)
        {
            ss << "|";
            ss << (*(this->pool[i])).toString();
        }
        ss << ")";
        ss << this->getRepeatString();
        return ss.str();
    }
    std::wstring toWString() override
    {
        std::wstringstream ss;
        ss << "(";
        ss << (*(this->pool[0])).toWString();
        for (int i = 1; i < this->pool.size(); i++)
        {
            ss << "|";
            ss << (*(this->pool[i])).toWString();
        }
        ss << ")";
        ss << this->getRepeatWString();
        return ss.str();
    }
};

template <class TIterator>
class NextNode : public PoolNode<TIterator>
{
private:
    typedef Vertex<TIterator> Vertex;
    typedef PoolNode<TIterator> PoolNode;
    typedef NextNode<TIterator> Next;
    typedef OrNode<TIterator> Or;

public:
    ~NextNode() final
    {
    }
    NextNode(std::initializer_list<Vertex *> vertices)
        : PoolNode(vertices)
    {
    }
    NextNode(Vertex *vertex, std::vector<Vertex *> vertices)
        : PoolNode(vertex, vertices)
    {
    }
    NextNode(std::vector<Vertex *> vertices, Vertex *vertex)
        : PoolNode(vertices, vertex)
    {
    }
    NextNode(std::vector<Vertex *> verticesL, std::vector<Vertex *> verticesR)
        : PoolNode(verticesL, verticesR)
    {
    }
    std::vector<Vertex *> getOrIngredient() override
    {
        std::vector<Vertex *> ret;
        ret.push_back(this);
        return ret;
    }
    std::vector<Vertex *> getNextIngredient() override
    {
        if (this->isNeedPacked())
        {
            std::vector<Vertex *> ret;
            ret.push_back(this);
            return ret;
        }
        else
        {
            return this->pool;
        }
    }
    Vertex &operator||(Vertex &other) override
    {
        Vertex *ret = new Or(this, other.getOrIngredient());
        return *ret;
    }
    Vertex &operator>>(Vertex &other) override
    {
        if (this->isNeedPacked())
        {
            Vertex *ret = new NextNode(this, other.getNextIngredient());
            return *ret;
        }
        Vertex *ret = new NextNode(this->pool, other.getNextIngredient());
        return *ret;
    }
    bool operator()(TIterator &iter) override
    {
        auto nextFunc = [&]() {
            bool vaild = true;
            TIterator fallback = iter;
            for (auto &v : this->pool)
            {
                vaild = (*v)(iter);
                if (!vaild)
                {
                    iter = fallback;
                    break;
                }
            }
            return vaild;
        };
        bool vaild = this->repeat(nextFunc);
        if (vaild && this->action)
        {
            this->action();
        }
        return vaild;
    }
    std::string toString() override
    {
        std::stringstream ss;
        for (int i = 0; i < this->pool.size(); i++)
        {
            ss << this->pool[i]->toString();
        }
        ss << this->getRepeatString();
        return ss.str();
    }
    std::wstring toWString() override
    {
        std::wstringstream ss;
        for (int i = 0; i < this->pool.size(); i++)
        {
            ss << this->pool[i]->toWString();
        }
        ss << this->getRepeatWString();
        return ss.str();
    }
};

template <class TIterator, class TLexValue>
class UnitNode : public Vertex<TIterator>
{
private:
    typedef Vertex<TIterator> Vertex;
    typedef Unit<TIterator, TLexValue> Unit;
    typedef NextNode<TIterator> Next;
    typedef OrNode<TIterator> Or;
    Unit *unit;

public:
    UnitNode(Unit *unit)
        : Vertex()
    {
        this->unit = unit;
    }
    std::vector<Vertex *> getOrIngredient() override
    {
        std::vector<Vertex *> ret;
        ret.push_back(this);
        return ret;
    }
    std::vector<Vertex *> getNextIngredient() override
    {
        std::vector<Vertex *> ret;
        ret.push_back(this);
        return ret;
    }
    Vertex &operator||(Vertex &other) override
    {
        Vertex *ret = new Or(this, other.getOrIngredient());
        return *ret;
    }
    Vertex &operator>>(Vertex &other) override
    {
        Vertex *ret = new Next(this, other.getNextIngredient());
        return *ret;
    }
    bool operator()(TIterator &iter) override
    {
        auto unitFunc = [&]() {
            return (*(this->unit))(iter);
        };

        bool vaild = this->repeat(unitFunc);
        if (vaild && this->action)
        {
            this->action();
        }
        return vaild;
    }
    std::wstring toWString() override
    {
        std::wstringstream ss;
        ss << (*(this->unit)).toWString();
        ss << this->getRepeatWString();
        return ss.str();
    }
    std::string toString() override
    {
        std::stringstream ss;
        ss << (*(this->unit)).toString();
        ss << this->getRepeatString();
        return ss.str();
    }
    ~UnitNode() final
    {
    }
};

} // namespace CompilerLib::Lexer

#endif