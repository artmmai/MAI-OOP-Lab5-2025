#pragma once
#include "my_memory_resource.h"
#include "vector_iterator.h"
#include <memory_resource>
#include <stdexcept>
#include <utility>

namespace my_vector {

template<typename T>
class PmrVector {
private:
    using allocator_type = std::pmr::polymorphic_allocator<T>;
    using traits_type    = std::allocator_traits<allocator_type>;

    T* data_ptr = nullptr;
    size_t vec_size = 0;
    size_t vec_capacity = 0;
    allocator_type allocator;

    // === РАСШИРЕНИЕ ЁМКОСТИ === 
    void reallocate(size_t new_cap) {
        T* new_block = allocator.allocate(new_cap);

        for (size_t i = 0; i < vec_size; i++)
            traits_type::construct(allocator, new_block + i, std::move(data_ptr[i]));

        for (size_t i = 0; i < vec_size; i++)
            traits_type::destroy(allocator, data_ptr + i);

        if (data_ptr)
            allocator.deallocate(data_ptr, vec_capacity);

        data_ptr = new_block;
        vec_capacity = new_cap;
    }

public:
    using Iterator = VectorIterator<T>;

    PmrVector(std::pmr::memory_resource* resource = std::pmr::get_default_resource())
        : allocator(resource) {}

    ~PmrVector() {
        clear();
        if(data_ptr) { 
            allocator.deallocate(data_ptr, vec_capacity);
        }
    }

    // === Запрешаем копирование и перемещение ===
    PmrVector(const PmrVector&) = delete;
    PmrVector& operator=(const PmrVector&) = delete;
    PmrVector(PmrVector&&) = delete;
    PmrVector& operator=(PmrVector&&) = delete;

    // === capacity ===
    size_t size() const {
        return vec_size; 
    }
    
    size_t capacity() const {
        return vec_capacity;
    }
    
    bool empty() const {
        return vec_size == 0;
    }
    
    void reserve(size_t new_cap) {
        if(new_cap > vec_capacity) {
            reallocate(new_cap);
        }
    }

    // === ДОСТУП К ЭЛЕМЕНТУ ===
    T& operator[](size_t index) { 
        return data_ptr[index];
    }
    
    const T& operator[](size_t index) const {
        return data_ptr[index];
    }

    T& at(size_t index) { 
        if(index >= vec_size) {
            throw std::out_of_range("index out of range"); 
        }
        return data_ptr[index]; 
    }
    const T& at(size_t index) const { 
        if(index >= vec_size) {
            throw std::out_of_range("index out of range");
        } 
        return data_ptr[index]; 
    }

    // === МОДИФИКАТОРЫ ===
    void push_back(const T& value) {
        if(vec_size == vec_capacity) reallocate(vec_capacity == 0 ? 2 : vec_capacity * 2);
        traits_type::construct(allocator, data_ptr + vec_size, value);
        vec_size++;
    }

    void pop_back() {
        if(empty()) { 
            throw std::runtime_error("pop_back from empty vector");
        }
        
        vec_size--;
        traits_type::destroy(allocator, data_ptr + vec_size);

    }

    void clear() {
        for(size_t i = 0; i < vec_size; i++){
            traits_type::destroy(allocator, data_ptr + i);
        }
        vec_size = 0;
    }

    // === front/back ===
    T& front() { 
        if(empty()) {
            throw std::runtime_error("front() from empty vector"); 

        }
        
        return data_ptr[0]; 
    }
    
    const T& front() const { 
        if(empty()) {
            throw std::runtime_error("front() from empty vector");
        } 
        
        return data_ptr[0]; 
    }

    T& back() { 
        if(empty()) {
            throw std::runtime_error("back() from empty vector"); 
        }
        
        return data_ptr[vec_size - 1]; 
    }
    
    const T& back() const { 
        if(empty()) { 
            throw std::runtime_error("back() from empty vector"); 
        }
        return data_ptr[vec_size-1]; 
    }

    // === ВСТАВКА/УДАЛЕНИЕ ПО ИНДЕКСУ ===
    void insert(size_t index, const T& value) {
        if(index > vec_size) {
            throw std::out_of_range("insert index out of range");
        }
        
        if(vec_size == vec_capacity) {
            reallocate(vec_capacity == 0 ? 2 : vec_capacity * 2);
        }

        for(size_t i = vec_size; i > index; i--) {
            traits_type::construct(allocator, data_ptr + i, std::move(data_ptr[i-1]));
            traits_type::destroy(allocator, data_ptr + i-1);
        }

        traits_type::construct(allocator, data_ptr + index, value);
        vec_size++;
    }

    void erase(size_t index) {
        if(index >= vec_size) { 
            throw std::out_of_range("erase index out of range");
        }

        traits_type::destroy(allocator, data_ptr + index);
        for (size_t i = index; i + 1 < vec_size; ++i) {
            data_ptr[i] = std::move(data_ptr[i + 1]);
        }
        vec_size--;
        traits_type::destroy(allocator, data_ptr + vec_size);
    }

    Iterator begin() {
        return Iterator(data_ptr); 
    }
    
    Iterator end() {
        return Iterator(data_ptr + vec_size);
    }
};
} // namespace my_vector
