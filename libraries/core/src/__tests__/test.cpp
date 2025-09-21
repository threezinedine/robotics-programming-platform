#include <gtest/gtest.h>
#include "platforms/memory.h"

RPP_ENABLE_MEMORY_TRACKING;

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}