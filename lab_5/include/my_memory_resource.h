#pragma once
#include <memory_resource>
#include <list>
#include <cstddef>
#include <new>

namespace my_vector {

struct BlockInfo {
    void* ptr;
    size_t size;
    size_t alignment; // выравнивание
    bool is_allocated; 
};

class ListMemoryResource : public std::pmr::memory_resource {
public:
    ListMemoryResource() = default;
    ~ListMemoryResource();

    ListMemoryResource(const ListMemoryResource&) = delete;
    ListMemoryResource& operator=(const ListMemoryResource&) = delete;

private:
    std::list<BlockInfo> blocks; // список блоков памяти

    void* do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void* p, size_t bytes, size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;
};

}
