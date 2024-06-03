#include "../Pointer.hpp"
#include <gtest/gtest.h>

TEST(PointerTest, construct_pointer_ows_nothing_test)
{
    Pointer<int> int_ptr;
    EXPECT_EQ(int_ptr.get(), nullptr);
    EXPECT_EQ(typeid(*int_ptr.get()), typeid(int));
    EXPECT_EQ(typeid(int_ptr.get_deleter()), typeid(deleter<int>));

    Pointer<char> char_ptr(nullptr);
    EXPECT_EQ(char_ptr.get(), nullptr);
    EXPECT_EQ(typeid(*char_ptr.get()), typeid(char));
    EXPECT_EQ(typeid(char_ptr.get_deleter()), typeid(deleter<char>));
}

TEST(PointerTest, construct_pointer_ows_diftypes_data_test)
{
    Pointer<int> u_int_ptr(new int(10));
    EXPECT_EQ(*u_int_ptr.get(), 10);
    EXPECT_EQ(typeid(*u_int_ptr.get()), typeid(int));

    char* char_ptr = new char('A');
    Pointer<char> u_char_ptr(char_ptr);
    EXPECT_EQ(*u_char_ptr.get(), 'A');
    EXPECT_EQ(typeid(*u_char_ptr.get()), typeid(char));
}

TEST(PointerTest, constructs_pointer_with_custom_deleter_test)
{
    struct custom_deleter
    {
        void operator()(int* ptr) { delete ptr; };
        const char* name = "custom_deleter";
    };

    custom_deleter dltr;

    Pointer<int, custom_deleter> u_int_ptr(new int(13), dltr);
    EXPECT_EQ(u_int_ptr.get_deleter().name, "custom_deleter");
    EXPECT_EQ(typeid(u_int_ptr.get_deleter()), typeid(custom_deleter));

    Pointer<char, void (*)(char*)> u_char_ptr(new char('A'),
                                              [](char* ptr) { delete ptr; });
    EXPECT_EQ(typeid(u_char_ptr.get_deleter()), typeid(void (*)(char*)));
}

TEST(PointerTest, constructs_pointer_by_transferring_ownership)
{
    Pointer<int> src(new int(13));
    auto src_ptr = src.get();

    auto dst(std::move(src));
    EXPECT_EQ(*dst, 13);
    EXPECT_EQ(dst.get(), src_ptr);
    EXPECT_EQ(src.get(), nullptr);

    src = std::move(dst);
    EXPECT_EQ(*src, 13);
    EXPECT_EQ(src.get(), src_ptr);
    EXPECT_EQ(dst.get(), nullptr);
}

TEST(PointerTest, release_pointer_test)
{
    Pointer<int> src(new int(13));
    auto src_ptr = src.get();
    auto dst = src.release();

    EXPECT_EQ(*dst, 13);
    EXPECT_EQ(dst, src_ptr);
    EXPECT_EQ(src.get(), nullptr);
}

TEST(PointerTest, reset_pointer_test)
{
    Pointer<int> ptr(new int(13));
    EXPECT_EQ(*ptr, 13);
    ptr.reset();
    EXPECT_EQ(ptr.get(), nullptr);

    ptr.reset(new int(31));
    EXPECT_EQ(*ptr, 31);
}

TEST(PointerTest, swap_pointer_test)
{
    Pointer<int> src(new int(13));
    Pointer<int> dst;
    auto src_ptr = src.get();
    dst.swap(src);
    EXPECT_EQ(*dst, 13);
    EXPECT_EQ(dst.get(), src_ptr);
    EXPECT_EQ(src.get(), nullptr);
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
    EXPECT_EQ(*int_ptr, 13);

    struct dummy
    {
        int _num = 13;
        const char* _str = "dummy";
    };

    Pointer<dummy> dummy_ptr(new dummy());
    EXPECT_EQ(dummy_ptr->_num, 13);
    EXPECT_EQ(dummy_ptr->_str, "dummy");
    EXPECT_EQ((*dummy_ptr)._num, 13);
    EXPECT_EQ((*dummy_ptr)._str, "dummy");
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}