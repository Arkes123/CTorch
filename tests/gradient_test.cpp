#include <gtest/gtest.h>
#include "../src/tensor.h"

int test1() {
    std::cout << "test" << std::endl;
    return 1;
}

TEST(ExampleTest, OneIsOne) {
    int result = test1();
    EXPECT_EQ(result, 1) << "Expected result to be 1, but got " << result;
}

int main(int argc, char **argv) {
    Tensor<float> test({3});
    Tensor<float> test2({3});

    test.prev_.print()
    test += test2;
    test.prev_.print()

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}