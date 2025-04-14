#include <gtest/gtest.h>

// A simple function to test
int add(int a, int b) {
    return a + b;
}

// Test case for the add function
TEST(HelloTest, BasicAssertions) {
    EXPECT_EQ(add(1, 1), 2);       // Should pass
    EXPECT_NE(add(2, 2), 5);       // Should also pass
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}