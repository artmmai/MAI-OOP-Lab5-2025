#pragma once
#include "my_memory_resource.h"
#include "vector_iterator.h"
#include <memory_resource>
#include <stdexcept>

namespace my_vector {

// === 4. Реализация шаблонного контейнера согласно варианту задания - динамический массив, === 
// === который использует созданный memory_resource через шаблон std::pmr::polymorphic_allocator === 
template<typename T>
class PmrVector {
private:
    using value_allocator_type  = std::pmr::polymorphic_allocator<T>;
    using value_traits = std::allocator_traits<value_allocator_type>;

    using pointer_allocator_type = std::pmr::polymorphic_allocator<T*>;
    using pointer_traits = std::allocator_traits<pointer_allocator_type>;

    T** pointers = nullptr;
    size_t vec_size = 0;
    size_t vec_capacity = 0;

    value_allocator_type val_alloc;
    pointer_allocator_type ptr_alloc;
    
    // === РАСШИРЕНИЕ ЁМКОСТИ === 
    void reallocate_pointers(size_t new_cap) {
        // выделяем новый блок для массива указателей (тип T*)
        T** new_table = ptr_alloc.allocate(new_cap);

        // копируем старые указатели
        for (size_t i = 0; i < vec_size; ++i) {
            new_table[i] = pointers ? pointers[i] : nullptr;
        }

        if (pointers) {
            ptr_alloc.deallocate(pointers, vec_capacity);
        }

        pointers = new_table;
        vec_capacity = new_cap;
    }

public:
    using Iterator = VectorIterator<T>;

    PmrVector(std::pmr::memory_resource* resource = std::pmr::get_default_resource()) : val_alloc(resource), ptr_alloc(resource) {}

    ~PmrVector() {
        clear();
        if (pointers) {
            ptr_alloc.deallocate(pointers, vec_capacity);
            pointers = nullptr;
            vec_capacity = 0;
        }
    }

    // === Запрешаем копирование и перемещение ===
    PmrVector(const PmrVector&) = delete;
    PmrVector& operator=(const PmrVector&) = delete;
    PmrVector(PmrVector&&) = delete;
    PmrVector& operator=(PmrVector&&) = delete;

    // === capacity ===
    size_t size() const noexcept {
        return vec_size; 
    }
    
    size_t capacity() const noexcept {
        return vec_capacity;
    }
    
    bool empty() const noexcept {
        return vec_size == 0; 
    }

    void reserve(size_t new_cap) {
        if (new_cap > vec_capacity) {
            reallocate_pointers(new_cap);
        }
    }

    // === ДОСТУП К ЭЛЕМЕНТУ ===
    T& operator[](size_t index) { 
        return *pointers[index]; 
    }
    
    const T& operator[](size_t index) const { 
        return *pointers[index]; 
    }

    T& at(size_t index) {
        if (index >= vec_size) {
            throw std::out_of_range("index out of range");
        }
        return *pointers[index];
    }

    const T& at(size_t index) const {
        if (index >= vec_size) {
            throw std::out_of_range("index out of range");
        }
        return *pointers[index];
    }

    // === МОДИФИКАТОРЫ ===
    void push_back(const T& value) {
        if (vec_size == vec_capacity) {
            size_t new_cap = (vec_capacity == 0) ? 2 : (vec_capacity * 2);
            reallocate_pointers(new_cap);
        }

        // выделяем место для объекта через value allocator
        T* p = val_alloc.allocate(1);
        try {
            value_traits::construct(val_alloc, p, value);
        } catch (...) {
            val_alloc.deallocate(p, 1);
            throw;
        }

        pointers[vec_size] = p;
        ++vec_size;
    }

    void pop_back() {
        if (empty()) { 
            throw std::runtime_error("pop_back from empty vector");
        }
        T* p = pointers[vec_size - 1];
        value_traits::destroy(val_alloc, p);
        val_alloc.deallocate(p, 1);
        pointers[vec_size - 1] = nullptr;
        --vec_size;
    }

    void clear() {
        for (size_t i = 0; i < vec_size; ++i) {
            if (pointers[i]) {
                value_traits::destroy(val_alloc, pointers[i]);
                val_alloc.deallocate(pointers[i], 1);
                pointers[i] = nullptr;
            }
        }
        vec_size = 0;
    }

    // === front/back ===
    T& front() {
        if (empty()) {
            throw std::runtime_error("front() from empty vector");
        }
        return *pointers[0];
    }

    const T& front() const {
        if (empty()) { 
            throw std::runtime_error("front() from empty vector");
        }
        return *pointers[0];
    }

    T& back() {
        if (empty()) {
            throw std::runtime_error("back() from empty vector");
        }
        return *pointers[vec_size - 1];
    }
    const T& back() const {
        if (empty()) {
            throw std::runtime_error("back() from empty vector");
        }
        return *pointers[vec_size - 1];
    }

    // === ВСТАВКА/УДАЛЕНИЕ ПО ИНДЕКСУ ===
    void insert(size_t index, const T& value) {
        if (index > vec_size) {
            throw std::out_of_range("insert index out of range");
        }
        if (vec_size == vec_capacity) {
            size_t new_cap = (vec_capacity == 0) ? 2 : (vec_capacity * 2);
            reallocate_pointers(new_cap);
        }

        // сдвигаем указатели вправо
        for (size_t i = vec_size; i > index; --i) {
            pointers[i] = pointers[i - 1];
        }

        // выделяем блок под новый элемент
        T* p = val_alloc.allocate(1);
        try {
            value_traits::construct(val_alloc, p, value);
        } catch (...) {
            val_alloc.deallocate(p, 1);
            for (size_t i = index; i < vec_size; ++i) { 
                pointers[i] = pointers[i + 1];
            }
            throw;
        }

        pointers[index] = p;
        ++vec_size;
    }

    void erase(size_t index) {
        if (index >= vec_size) { 
            throw std::out_of_range("erase index out of range");
        }

        T* p = pointers[index];
        value_traits::destroy(val_alloc, p);
        val_alloc.deallocate(p, 1);

        for (size_t i = index; i + 1 < vec_size; ++i) {
            pointers[i] = pointers[i + 1];
        }
        pointers[vec_size - 1] = nullptr;
        --vec_size;
    }

    Iterator begin() { 
        return Iterator(pointers);
    }
    Iterator end() { 
        return Iterator(pointers + vec_size);
    }
};
} // namespace my_vector
