#include "applications/applications.h"
#include <gtest/gtest.h>

RPP_ENABLE_MEMORY_TRACKING;

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}