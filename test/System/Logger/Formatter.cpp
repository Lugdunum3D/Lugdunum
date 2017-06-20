#include <lug/System/Logger/Formatter.hpp>
#include <lug/System/Logger/Common.hpp>
#include <lug/System/Logger/Message.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace lug {
namespace System {
namespace Logger {

using namespace ::testing;

TEST(Formatter, Formats) {
    Formatter formatter("[%v]\n");

    priv::Message msg("Test", Level::Info);
    msg.raw << "Hello world!";

    formatter.format(msg);

    ASSERT_STREQ(msg.formatted.c_str(), "[Hello world!]\n");
}

TEST(Formatter, FormatsWithLevel) {
    Formatter formatter("[%l][%v]\n");

    priv::Message msg("Test", Level::Info);
    msg.raw << "Hello world!";

    auto testOneLevel = [&](Level level, std::string stringified) {
        msg.level = level;
        formatter.format(msg);
        ASSERT_STREQ(msg.formatted.c_str(), ("[" + stringified + "][Hello world!]\n").c_str());
    };

    testOneLevel(Level::Debug,   "DEBUG  ");
    testOneLevel(Level::Info,    "INFO   ");
    testOneLevel(Level::Warning, "WARNING");
    testOneLevel(Level::Error,   "ERROR  ");
    testOneLevel(Level::Fatal,   "FATAL  ");
    testOneLevel(Level::Assert,  "ASSERT ");
    testOneLevel(static_cast<Level>(42), "UNKNOWN");
}

TEST(Formatter, FormatsTime) {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(now);
    std::tm tf;

    Formatter formatter("%y-%Y-%m-%d-%H-%M-%S\n");

    priv::Message msg("", Level::Info);

    #if defined(LUG_SYSTEM_WINDOWS)
        // Use windows secure versions of localtime
        localtime_s(&tf, &tt);
    #else
        // Use linux secure versions of localtime
        // TODO: test on android
        // It could not work (what is the localtime secure version for android ?)
        localtime_r(&tt, &tf);
    #endif

    formatter.format(msg, &tf);

    std::stringstream ss;
    ss << std::put_time(&tf, "%y-%Y-%m-%d-%H-%M-%S\n");

    ASSERT_STREQ(msg.formatted.c_str(), ss.str().c_str());
}

}
}
}
