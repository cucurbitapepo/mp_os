#include <not_implemented.h>

#include "../include/allocator_global_heap.h"

allocator_global_heap::allocator_global_heap(
    logger *logger)
    : _logger(logger)
{
}

allocator_global_heap::~allocator_global_heap()
{
}

allocator_global_heap::allocator_global_heap(
    allocator_global_heap &&other) noexcept
    : _logger(other._logger)
{
    other._logger = nullptr;
}

allocator_global_heap &allocator_global_heap::operator=(
    allocator_global_heap &&other) noexcept
{
    if(this != &other)
    {
        this->_logger = other._logger;
        other._logger = nullptr;
    }
    return *this;
}

[[nodiscard]] void *allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
    size_t total_size = value_size * values_count;
    debug_with_guard("requested size: " + std::to_string(total_size));
    void *ptr;
    try
    {
        ptr = ::operator new(total_size + sizeof(allocator*) + sizeof(size_t));
    }
    catch (std::bad_alloc const &ex)
    {
        error_with_guard("error allocating memory");
        throw;
    }

    *reinterpret_cast<allocator **>(ptr) = this;
    *reinterpret_cast<size_t *>(reinterpret_cast<unsigned char *>(ptr) + sizeof(allocator *)) = total_size;

    information_with_guard(std::to_string(total_size) + " bytes was allocated");
    return (reinterpret_cast<unsigned char *>(ptr) + sizeof(allocator *) + sizeof(size_t));

}

void allocator_global_heap::deallocate(
    void *at)
{
    void *to_clear = reinterpret_cast<unsigned char *>(at) - sizeof(size_t) - sizeof(allocator *);

    if(*reinterpret_cast<allocator **>(to_clear) != this)
    {
        error_with_guard("unable to deallocate a block that was allocated with other instance");
        throw std::logic_error("unable to deallocate a block that was allocated with other instance");
    }

    size_t total_size = *reinterpret_cast<size_t *>(reinterpret_cast<allocator **>(to_clear) + 1);

    std::string result;

    auto debug_ptr = reinterpret_cast<unsigned char *>(at);
    for(int i = 0; i < total_size; ++i)
    {
        result += std::to_string(debug_ptr[i]);
    }

    debug_with_guard("array of bytes: " + result);
    information_with_guard("memory deallocated");
    ::operator delete(to_clear);
}

inline logger *allocator_global_heap::get_logger() const
{
    return _logger;
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
    return typeid(*this).name();
}