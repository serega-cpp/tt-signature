#ifndef __MEMORY_BLOCK_HPP__
#define __MEMORY_BLOCK_HPP__

struct MemoryBlock
{
    std::unique_ptr<char[]> ptr;

    size_t size;
    size_t id;

    MemoryBlock(std::unique_ptr<char[]> &&ptr, size_t size, size_t id) noexcept
        : ptr(std::move(ptr))
        , size(size)
        , id(id) 
    {
    }

    MemoryBlock(MemoryBlock &&src) noexcept
        : ptr(std::move(src.ptr))
        , size(src.size)
        , id(src.id)
    {
    }

    MemoryBlock &operator = (MemoryBlock &&src) noexcept
    {
        ptr = std::move(src.ptr);
        size = src.size;
        id = src.id;

        return *this;
    }

    MemoryBlock(const MemoryBlock &) = delete;
    MemoryBlock &operator = (const MemoryBlock &src) = delete;
};

#endif // __MEMORY_BLOCK_HPP__
