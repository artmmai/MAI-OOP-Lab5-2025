#include "../include/vector.h"
#include "../include/vector_iterator.h"
#include <gtest/gtest.h>
#include <string>

class PmrVectorTest : public ::testing::Test {
protected:
    void SetUp() override {
        resource = new my_vector::ListMemoryResource();
        vec = new my_vector::PmrVector<int>(resource);
    }

    void TearDown() override {
        delete vec;
        delete resource;
    }

    my_vector::ListMemoryResource* resource;
    my_vector::PmrVector<int>* vec;
};

// === 1. Конструктор по умолчанию и пустой вектор ===
TEST_F(PmrVectorTest, DefaultConstructor) {
    EXPECT_TRUE(vec->empty());
    EXPECT_EQ(vec->size(), 0);
}

// === 2. push_back и увеличение размера ===
TEST_F(PmrVectorTest, PushBackIncreasesSize) {
    vec->push_back(1);
    EXPECT_EQ(vec->size(), 1);
    EXPECT_FALSE(vec->empty());

    vec->push_back(2);
    EXPECT_EQ(vec->size(), 2);
}

// === 3. Проверка front() и back() ===
TEST_F(PmrVectorTest, FrontAndBack) {
    vec->push_back(10);
    vec->push_back(20);
    vec->push_back(30);

    EXPECT_EQ(vec->front(), 10);
    EXPECT_EQ(vec->back(), 30);
}

// === 4. pop_back() уменьшает размер ===
TEST_F(PmrVectorTest, PopBackDecreasesSize) {
    vec->push_back(1);
    vec->push_back(2);
    vec->push_back(3);

    vec->pop_back();
    EXPECT_EQ(vec->size(), 2);
    EXPECT_EQ(vec->back(), 2);

    vec->pop_back();
    EXPECT_EQ(vec->size(), 1);
    EXPECT_EQ(vec->back(), 1);
}

// === 5. Попытка pop_back() на пустом векторе ===
TEST_F(PmrVectorTest, PopBackFromEmptyThrows) {
    EXPECT_THROW(vec->pop_back(), std::runtime_error);
}

// === 6. clear() очищает вектор ===
TEST_F(PmrVectorTest, ClearMakesEmpty) {
    vec->push_back(1);
    vec->push_back(2);
    vec->push_back(3);

    EXPECT_FALSE(vec->empty());
    vec->clear();
    EXPECT_TRUE(vec->empty());
    EXPECT_EQ(vec->size(), 0);
}

// === 7. Проверка работы итератора ===
TEST_F(PmrVectorTest, IteratorBasic) {
    vec->push_back(1);
    vec->push_back(2);
    vec->push_back(3);

    auto it = vec->begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    it++;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(it, vec->end());
}

// === 8. Range-based for ===
TEST_F(PmrVectorTest, IteratorRangeBasedFor) {
    vec->push_back(1);
    vec->push_back(2);
    vec->push_back(3);

    int sum = 0;
    for (int value : *vec) {  // В новой реализации vec сам по себе поддерживает begin/end
        sum += value;
    }
    EXPECT_EQ(sum, 6);
}

// === 9. Работа со сложными типами (std::string) ===
TEST(PmrVectorComplexTest, ComplexTypePushBack) {
    my_vector::ListMemoryResource resource;
    my_vector::PmrVector<std::string> vec(&resource);

    vec.push_back("Valentin Zaitsev");
    vec.push_back("Vladimir Putin");

    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.front(), "Valentin Zaitsev");
    EXPECT_EQ(vec.back(), "Vladimir Putin");
}

// === 10. Итератор для сложных типов ===
TEST(PmrVectorComplexTest, ComplexTypeIterator) {
    my_vector::ListMemoryResource resource;
    my_vector::PmrVector<std::string> vec(&resource);

    vec.push_back("Valentin");
    vec.push_back("Vladimir");
    vec.push_back("Donald");

    std::string concat;
    for (const auto& s : vec) {
        concat += s + " ";
    }

    EXPECT_EQ(concat, "Valentin Vladimir Donald ");
}

// === 11. Исключения для front/back пустого вектора ===
TEST(PmrVectorExceptionTest, FrontBackEmptyThrows) {
    my_vector::ListMemoryResource resource;
    my_vector::PmrVector<int> vec(&resource);

    EXPECT_THROW(vec.front(), std::runtime_error);
    EXPECT_THROW(vec.back(), std::runtime_error);
}

// === 12. Проверка повторного использования памяти ===
TEST_F(PmrVectorTest, ReuseDeallocatedMemory) {
    // Добавляем элементы
    vec->push_back(1);
    vec->push_back(2);
    vec->push_back(3);

    // Сохраняем адреса
    int* ptr1 = &(*vec)[0];
    int* ptr2 = &(*vec)[1];

    // Удаляем первый элемент
    vec->erase(0);

    // Добавляем новый элемент
    vec->push_back(42);

    // Проверяем, что память первого удалённого элемента могла быть переиспользована
    EXPECT_TRUE(ptr1 == &(*vec)[1] || ptr2 == &(*vec)[0]);
}
