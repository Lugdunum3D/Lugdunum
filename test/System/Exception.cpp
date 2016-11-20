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
