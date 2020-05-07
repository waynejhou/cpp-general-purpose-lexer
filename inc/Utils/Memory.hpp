#ifndef UTILS_MEMORY_HPP
#define UTILS_MEMORY_HPP

#include <cstddef>
#include <string>

namespace Utils::Memory
{
class Arena
{
public:
    Arena(size_t size);
    template <typename T>
    T *Allocate(T);

private:
    void *pointer;
    void *top;
    size_t size;
};
} // namespace Memory

#endif