#include <lug/System/Logger/Formatter.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace lug {
namespace System {

using namespace ::testing;

TEST(Formatter, Formats) {
    Formatter formatter("[%v]\n");

    priv::Message msg("Test", Level::Info);
    msg.raw << "Hello world!";

    formatter.format(msg);

    ASSERT_STREQ(msg.formatted.c_str(), "[Hello world!]\n");
}

}
}
