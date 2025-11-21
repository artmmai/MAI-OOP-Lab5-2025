#pragma once
#include <cstddef>
#include <iterator>

namespace my_vector {

// === 5. Реализация итератора к созданному контейнеру (дин. массив) ===
// === Итератор соответствует std::forward_iterator_tag ===
template<typename T>
class VectorIterator {
private:
    // Итератор хранит позицию внутри динамического массива
    // Храним указатель на указатель - элемент хранится как T*
    T** current; 

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    explicit VectorIterator(T** ptr) : current(ptr) {}

    // Разыменование итератора
    reference operator*() const {
        return **current; // разыменовываем указатель на объект
    }

    pointer operator->() const {
        return *current; // возвращаем указатель на объект
    }

    // Инкрементирование итератора
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

    // Сравнение итераторов
    bool operator==(const VectorIterator& other) const {
        return current == other.current;
    }

    bool operator!=(const VectorIterator& other) const {
        return current != other.current;
    }
};

} // namespace my_vector
