#include "gtest/gtest.h"

// TODO need to disable network test to prevent unstability
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}