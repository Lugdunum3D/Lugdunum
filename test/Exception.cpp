#include <lug/Config.hpp>
#include <lug/System/Exception.hpp>
#include <gtest/gtest.h>

TEST(ExceptionReturnValues, ReturnLine) {
    try {
        LUG_EXCEPT(IOException, "My exception worked");
    }
    catch (const lug::System::IOException &e) {
        EXPECT_EQ(e.getLine(), 7);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
