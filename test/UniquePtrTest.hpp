#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../UniquePtr.hpp"
#include "MockDeleter.hpp"

TEST(UniquePtrTest, construct_UniquePtr_ows_nothing_test)
{
    {
        UniquePtr<int> ptr;
        EXPECT_EQ(ptr.get(), nullptr);
    }
    // No Deleter call - nothing to delete.

    UniquePtr<char> ptr(nullptr);
    EXPECT_EQ(ptr.get(), nullptr);
}

template <typename T>
void check_UniquePtr(T value)
{
    UniquePtr<T> ptr(new T(value));
    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, value);
}

TEST(UniquePtrTest, construct_UniquePtr_ows_diftypes_data_test)
{
    check_UniquePtr(10);
    check_UniquePtr(1.1);
    check_UniquePtr('A');
    check_UniquePtr("Avada Kedavra");
}

template <typename T>
void check_UniquePtr(const UniquePtr<T>& ptr, T value)
{
    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, value);
}

TEST(UniquePtrTest, constructs_UniquePtr_with_custom_deleter_test)
{
    {
        MockDeleter<int> dltr;
        UniquePtr<int> ptr(new int(13), std::move(dltr));
        check_UniquePtr(ptr, 13);
        // EXPECT_CALL(ptr.get_deleter(), deleter_call(testing::_)); ??
    }

    {
        UniquePtr<char> ptr(new char('A'), [](char* ptr) { delete ptr; });
        check_UniquePtr(ptr, 'A');
        // EXPECT_CALL(ptr.get_deleter(), ); ??
    }
}

template <typename T>
void check_UniquePtr_move(const UniquePtr<T>& src, const UniquePtr<T>& dst)
{
    ASSERT_NE(dst.get(), nullptr);
    EXPECT_EQ(*dst, 13);
    EXPECT_EQ(src.get(), nullptr);
};

TEST(UniquePtrTest, constructs_UniquePtr_by_transferring_ownership)
{
    UniquePtr<int> src(new int(13));
    auto src_ptr = src.get();
    auto dst(std::move(src));
    check_UniquePtr_move(src, dst);
    EXPECT_EQ(dst.get(), src_ptr);

    src = std::move(dst);
    check_UniquePtr_move(dst, src);
    EXPECT_EQ(src.get(), src_ptr);
}

TEST(UniquePtrTest, release_UniquePtr_test)
{
    UniquePtr<int> src(new int(13));
    auto src_ptr = src.get();
    auto dst = src.release();

    EXPECT_EQ(*dst, 13);
    EXPECT_EQ(dst, src_ptr);
    EXPECT_EQ(src.get(), nullptr);
}

TEST(UniquePtrTest, reset_UniquePtr_test)
{
    UniquePtr<int> ptr(new int(13), std::move(MockDeleter<int>()));

    check_UniquePtr(ptr, 13);
    // EXPECT_CALL(ptr.get_deleter(), deleter_call(::testing::_)).Times(2);
    ptr.reset();
    EXPECT_EQ(ptr.get(), nullptr);

    ptr.reset(new int(31));
    check_UniquePtr(ptr, 31);
}

TEST(UniquePtrTest, swap_UniquePtr_test)
{
    UniquePtr<int> src(new int(13));
    UniquePtr<int> dst;
    auto src_ptr = src.get();
    dst.swap(src);
    EXPECT_EQ(*dst, 13);
    EXPECT_EQ(dst.get(), src_ptr);
    EXPECT_EQ(src.get(), nullptr);
}

TEST(UniquePtrTest, UniquePtr_operator_bool_test)
{
    UniquePtr<int> int_ptr;
    EXPECT_FALSE(int_ptr);

    int_ptr.reset(nullptr);
    EXPECT_FALSE(int_ptr);

    UniquePtr<char> char_ptr(new char('A'));
    EXPECT_TRUE(char_ptr);
}

TEST(UniquePtrTest, UniquePtr_access_operator_test)
{
    UniquePtr<int> int_ptr(new int(13));
    ASSERT_NE(int_ptr.get(), nullptr);
    EXPECT_EQ(*int_ptr, 13);

    struct dummy
    {
        int _num = 13;
        const char* _str = "dummy";
    };

    UniquePtr<dummy> dummy_ptr(new dummy());
    ASSERT_NE(dummy_ptr.get(), nullptr);
    EXPECT_EQ(dummy_ptr->_num, 13);
    EXPECT_EQ(dummy_ptr->_str, "dummy");
    EXPECT_EQ((*dummy_ptr)._num, 13);
    EXPECT_EQ((*dummy_ptr)._str, "dummy");
}
