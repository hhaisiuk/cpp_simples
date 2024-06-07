#pragma once
#include "../PointerTuple.hpp"
#include "MockDeleter.hpp"

TEST(PointerTest, construct_pointer_ows_nothing_test)
{
    {
        Pointer<int, MockDeleter<int>> int_ptr;
        EXPECT_EQ(int_ptr.get(), nullptr);
    }
    // No Deleter call - nothing to delete.

    Pointer<char> char_ptr(nullptr);
    EXPECT_EQ(char_ptr.get(), nullptr);
}

template <typename T>
void check_pointer(T value)
{
    Pointer<T> ptr(new T(value));
    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, value);
}

TEST(PointerTest, construct_pointer_ows_diftypes_data_test)
{
    check_pointer(10);
    check_pointer(1.1);
    check_pointer('A');
    check_pointer("Avada Kedavra");
}

template <typename T, typename D>
void check_pointer(const Pointer<T, D>& ptr, T value)
{
    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, value);
}

TEST(PointerTest, constructs_pointer_with_custom_deleter_test)
{
    {
        MockDeleter<int> dltr;
        Pointer<int, MockDeleter<int>> ptr(new int(13), std::move(dltr));
        check_pointer(ptr, 13);
        EXPECT_CALL(ptr.get_deleter(), deleter_call(testing::_));
    }

    {
        Pointer<char, void (*)(char*)> ptr(new char('A'),
                                           [](char* ptr) { delete ptr; });
        check_pointer(ptr, 'A');
    }
}

template <typename T>
void check_pointer(const Pointer<T>& src, const Pointer<T>& dst, T value,
                   T* ptr)
{
    ASSERT_NE(dst.get(), nullptr);
    EXPECT_EQ(*dst, value);
    EXPECT_EQ(src.get(), nullptr);
    EXPECT_EQ(dst.get(), ptr);
};

TEST(PointerTest, constructs_pointer_by_transferring_ownership)
{
    int expected_value = 13;
    Pointer<int> src(new int(expected_value));
    auto src_ptr = src.get();
    auto dst(std::move(src));
    check_pointer(src, dst, expected_value, src_ptr);

    src = std::move(dst);
    check_pointer(dst, src, expected_value, src_ptr);
}

TEST(PointerTest, release_pointer_test)
{
    Pointer<int> src(new int(13));
    auto src_ptr = src.get();
    auto dst = src.release();

    ASSERT_NE(dst, nullptr);
    EXPECT_EQ(*dst, 13);
    EXPECT_EQ(dst, src_ptr);
    EXPECT_EQ(src.get(), nullptr);
}

TEST(PointerTest, reset_pointer_test)
{
    Pointer<int, MockDeleter<int>> ptr(new int(13),
                                       std::move(MockDeleter<int>()));

    check_pointer(ptr, 13);
    EXPECT_CALL(ptr.get_deleter(), deleter_call(::testing::_)).Times(2);
    ptr.reset();
    EXPECT_EQ(ptr.get(), nullptr);

    ptr.reset(new int(31));
    check_pointer(ptr, 31);
}

TEST(PointerTest, swap_pointer_test)
{
    int expected_value = 13;
    Pointer<int> src(new int(expected_value));
    Pointer<int> dst;
    auto src_ptr = src.get();
    dst.swap(src);

    check_pointer(src, dst, expected_value, src_ptr);
}

TEST(PointerTest, pointer_operator_bool_test)
{
    Pointer<int> int_ptr;
    EXPECT_FALSE(int_ptr);

    int_ptr.reset(nullptr);
    EXPECT_FALSE(int_ptr);

    Pointer<char> char_ptr(new char('A'));
    EXPECT_TRUE(char_ptr);
}

TEST(PointerTest, pointer_access_operator_test)
{
    Pointer<int> int_ptr(new int(13));
    ASSERT_NE(int_ptr.get(), nullptr);
    EXPECT_EQ(*int_ptr, 13);

    struct dummy
    {
        int _num = 13;
        const char* _str = "dummy";
    };

    Pointer<dummy> dummy_ptr(new dummy());
    ASSERT_NE(dummy_ptr.get(), nullptr);
    EXPECT_EQ(dummy_ptr->_num, 13);
    EXPECT_EQ(dummy_ptr->_str, "dummy");
    EXPECT_EQ((*dummy_ptr)._num, 13);
    EXPECT_EQ((*dummy_ptr)._str, "dummy");
}
