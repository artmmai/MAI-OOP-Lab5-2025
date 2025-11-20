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

TEST_F(PmrVectorTest, DefaultConstructor) {
    EXPECT_TRUE(vec->empty());
    EXPECT_EQ(vec->size(), 0);
}

TEST_F(PmrVectorTest, PushBackIncreasesSize) {
    vec->push_back(1);
    EXPECT_EQ(vec->size(), 1);
    EXPECT_FALSE(vec->empty());

    vec->push_back(2);
    EXPECT_EQ(vec->size(), 2);
}

TEST_F(PmrVectorTest, FrontAndBack) {
    vec->push_back(10);
    vec->push_back(20);
    vec->push_back(30);

    EXPECT_EQ(vec->front(), 10);
    EXPECT_EQ(vec->back(), 30);
}

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

TEST_F(PmrVectorTest, PopBackFromEmptyThrows) {
    EXPECT_THROW(vec->pop_back(), std::runtime_error);
}

TEST_F(PmrVectorTest, ClearMakesEmpty) {
    vec->push_back(1);
    vec->push_back(2);
    vec->push_back(3);

    EXPECT_FALSE(vec->empty());
    vec->clear();
    EXPECT_TRUE(vec->empty());
    EXPECT_EQ(vec->size(), 0);
}

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

TEST_F(PmrVectorTest, IteratorRangeBasedFor) {
    vec->push_back(1);
    vec->push_back(2);
    vec->push_back(3);

    int sum = 0;
    for (int value : *vec) {
        sum += value;
    }
    EXPECT_EQ(sum, 6);
}

TEST(PmrVectorComplexTest, ComplexTypePushBack) {
    my_vector::ListMemoryResource resource;
    my_vector::PmrVector<std::string> vec(&resource);

    vec.push_back("Alice");
    vec.push_back("Bob");

    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.front(), "Alice");
    EXPECT_EQ(vec.back(), "Bob");
}

TEST(PmrVectorComplexTest, ComplexTypeIterator) {
    my_vector::ListMemoryResource resource;
    my_vector::PmrVector<std::string> vec(&resource);

    vec.push_back("Alice");
    vec.push_back("Bob");
    vec.push_back("Charlie");

    std::string concat;
    for (const auto& s : vec) {
        concat += s + " ";
    }

    EXPECT_EQ(concat, "Alice Bob Charlie ");
}

TEST(PmrVectorExceptionTest, FrontBackEmptyThrows) {
    my_vector::ListMemoryResource resource;
    my_vector::PmrVector<int> vec(&resource);

    EXPECT_THROW(vec.front(), std::runtime_error);
    EXPECT_THROW(vec.back(), std::runtime_error);
}
