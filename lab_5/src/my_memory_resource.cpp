#include "../include/my_memory_resource.h"
#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace my_vector {

// Деструктор: освобождает все блоки памяти 
ListMemoryResource::~ListMemoryResource() {
    for (const BlockInfo& block : blocks) {
        std::free(block.ptr);
        std::cout << "Cleaned up memory at " << block.ptr << " (size: " << block.size << " bytes)\n";
    }
    blocks.clear();
}

void* ListMemoryResource::do_allocate(size_t bytes, size_t alignment) {
    for (BlockInfo& block : blocks) {
        // === 2. Повторное переиспользование ранее освобождённой памяти ===
        if (!block.is_allocated && block.size >= bytes && block.alignment >= alignment) {
            block.is_allocated = true;
            std::cout << "REUSED memory at " << block.ptr << " for " << bytes << " bytes\n";
            return block.ptr;
        }
    }

    // Выделение нового блока
    void* ptr = std::aligned_alloc(alignment, bytes);
    if (!ptr) { 
        throw std::bad_alloc();
    }

    blocks.push_back({ptr, bytes, alignment, true});
    std::cout << "ALLOCATED new memory at " << ptr << " for " << bytes << " bytes\n";
    return ptr;
}

// Освобождение памяти
void ListMemoryResource::do_deallocate(void* ptr, size_t bytes, size_t alignment) {
    auto it = std::find_if(
        blocks.begin(),
        blocks.end(),
        [ptr](const BlockInfo& b) {
            return b.ptr == ptr; 
        });

    if (it != blocks.end() && it->is_allocated) {
        it->is_allocated = false;
        std::cout << "DEALLOCATED memory at " << ptr << " (marked as free for reuse)\n";
    } else {
        throw std::runtime_error("Trying to deallocate non-allocated pointer");
    }
}

// Сравнение memory_resource
bool ListMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}

} // namespace my_vector
