#pragma once

#include <cstddef>
#include <iterator>
namespace my_vector {

template<typename T>
class VectorIterator {
private:
    T* current;

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    explicit VectorIterator(T* ptr) : current(ptr) {}

    reference operator*() {
        return *current;
    }
    
    pointer operator->() {
        return current;
    }

    // ++it
    VectorIterator& operator++() {
        ++current;
        return *this;
    }

    // it++
    VectorIterator operator++(int) {
        VectorIterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const VectorIterator& other) const {
        return current == other.current;
    }

    bool operator!=(const VectorIterator& other) const {
        return current != other.current;
    }
};

} // namespace my_vector
