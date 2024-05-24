#include "../Pointer.hpp"
#include "TestCommon.hpp"

/**
 * @test 
 *  pointer_construction_test
 * @brief 
 *  check normal case of pointer constrution
 * @details
 *  create pointer obj of different types and check obj internal data 
*/
int pointer_construction_test(int, char**)
{
    // case 1: create Pointer<int> 
    const int exp_size = 10;
    const int exp_alloc_mem = sizeof(int) * exp_size;
    Pointer<int> int_ptr(exp_size);
    EXPECT_EQ(int_ptr.size(), exp_size);
    EXPECT_NE(int_ptr.data(), nullptr);

    return 0;
}

