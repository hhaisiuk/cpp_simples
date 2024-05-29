#include <gtest/gtest.h>
#include "../Pointer.hpp"

// common check functions
template <typename T>
void check_equal(Pointer<T> &rhs, Pointer<T> &lhs) {
    EXPECT_EQ(rhs.size(), lhs.size());
    for(auto rhs_iter = rhs.data(), lhs_iter = lhs.data(); 
        rhs_iter != rhs.data() + rhs.size() && lhs_iter != lhs.data()+ lhs.size(); 
        rhs_iter++, lhs_iter++)
            EXPECT_EQ((*rhs_iter), (*lhs_iter));
}

/**
 * @test pointer_construction_test
 * @brief Construct pointer to manage different types data.
*/
TEST(PointerTest, pointer_construction_test)
{
    Pointer<int> int_ptr(10);
    EXPECT_EQ(int_ptr.size(), 10);
    EXPECT_NE(int_ptr.data(), nullptr);
    EXPECT_EQ(typeid((*int_ptr.data())), typeid(int));

    Pointer<char> char_ptr(20);
    EXPECT_EQ(char_ptr.size(), 20);
    EXPECT_NE(char_ptr.data(), nullptr);
    EXPECT_EQ(typeid((*char_ptr.data())), typeid(char));
}

/**
 * @test pointer_construction_invalid_values
 * @brief Construct a "zero" pointer and pointer with negative size
*/
TEST(PointerTest, pointer_construction_invalid_values)
{
    Pointer<int> zero_ptr(0);
    EXPECT_EQ(zero_ptr.size(), 0);
    EXPECT_EQ(zero_ptr.data(), nullptr);

    EXPECT_THROW(Pointer<int>(-1), std::bad_alloc);
}

/**
 * @test pointer_construction_default_values_test
 * @brief Construct a "zero" pointer and pointer with negative size
*/
TEST(PointerTest, pointer_construction_default_values_test)
{
    Pointer<int> ptr(10, 13);
    EXPECT_EQ(ptr.size(), 10);
    EXPECT_NE(ptr.data(), nullptr);
    for(auto iter = ptr.data(); iter != ptr.data() + ptr.size(); iter++)
        EXPECT_EQ((*iter), 13);
}

/**
 * @test pointer_copy_constructor_test
 * @brief Init Pointer obj via Pointer copy constructor
*/
TEST(PointerTest, pointer_copy_constructor_test)
{
    Pointer<int> src(5, 13);
    auto dst(src);

    EXPECT_NE(src.data(), dst.data());
    check_equal(src, dst);

    size_t exp_size = 10; 
    int exp_def_data_value = 'A';
    auto char_ptr = Pointer<char>(exp_size, exp_def_data_value);
    EXPECT_EQ(char_ptr.size(), exp_size);
    EXPECT_NE(char_ptr.data(), nullptr);
    for(auto iter = char_ptr.data(); iter != char_ptr.data() + char_ptr.size(); iter++)
        EXPECT_EQ((*iter), exp_def_data_value);
}

/**
 * @test pointer_copy_assignment_opererator_test
 * @brief Init Pointer obj via Pointer copy assignment opererator=
*/
TEST(PointerTest, pointer_copy_assignment_opererator_test)
{
    Pointer<int> src(5, 13);
    Pointer<int> dst(0);    
    dst = src;

    EXPECT_NE(src.data(), dst.data());
    check_equal(src, dst);  
}

/**
 * @test pointer_move_constructor_test
 * @brief Init Pointer obj via Pointer move constructor
*/
TEST(PointerTest, pointer_move_constructor_test)
{
    size_t exp_size = 10; 
    int exp_def_data_value = 'A';

    Pointer<int> src(exp_size, exp_def_data_value);
    Pointer<int> dst(std::move(src));

    EXPECT_EQ(dst.size(), exp_size);
    EXPECT_NE(dst.data(), nullptr);
    for(auto iter = dst.data(); iter != dst.data() + dst.size(); iter++)
        EXPECT_EQ((*iter), exp_def_data_value);
    
    EXPECT_EQ(src.data(), nullptr);
}

/**
 * @test pointer_move_assignment_opererator_test
 * @brief Init Pointer obj via Pointer move assignment opererator=
*/
TEST(PointerTest, pointer_move_assignment_opererator_test)
{
    size_t exp_size = 10; 
    int exp_def_data_value = 'A';

    Pointer<int> src(exp_size, exp_def_data_value);
    Pointer<int> dst(0);
    dst = std::move(src);

    EXPECT_EQ(dst.size(), exp_size);
    EXPECT_NE(dst.data(), nullptr);
    for(auto iter = dst.data(); iter != dst.data() + dst.size(); iter++)
        EXPECT_EQ((*iter), exp_def_data_value);

    EXPECT_EQ(src.data(), nullptr);
}


int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}