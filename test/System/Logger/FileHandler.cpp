#include <lug/System/Logger/FileHandler.hpp>
#include <lug/System/Logger/Message.hpp>
#include <lug/System/Logger/Formatter.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdio>

namespace lug {
namespace System {
namespace Logger {

using namespace ::testing;

constexpr const char* handlerName = "MyTestHandler";
const std::string fileName = "LugdunumTestFile.txt";

TEST(FileHandler, CallsFormat) {
    class MockFormatter : public Formatter {
    public:
        using Formatter::Formatter;
        MOCK_METHOD1(format, void(priv::Message& msg));
    };

    std::unique_ptr<MockFormatter> formatter = std::make_unique<MockFormatter>("");

    EXPECT_CALL(*(formatter.get()), format(AllOf(
        Field(&priv::Message::loggerName, StrEq("Test")),
        Field(&priv::Message::level, Level::Info),
        Field(&priv::Message::raw, Property(&fmt::MemoryWriter::c_str, StrEq("Hello world!")))
    ))).Times(1);

    Handler* handler = makeHandler<FileHandler>(handlerName, "toto", true);
    handler->setFormatter(std::move(formatter));
    priv::Message msg("Test", Level::Info);
    msg.raw << "Hello world!";
    handler->format(msg);

    LoggingFacility::clear();
}

TEST(FileHandler, Test) {
    Handler* handler = makeHandler<FileHandler>(handlerName, fileName, true);
    priv::Message msg("Test", Level::Info);
    msg.raw << "Hello world!";
    handler->setPattern("%v\n");
    handler->format(msg);
    handler->handle(msg);
    handler->flush();

    std::ifstream ifs;
    ifs.open(fileName);

    if (ifs.is_open()) {
        std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        ASSERT_STREQ(content.c_str(), "Hello world!\n");
    } else {
        FAIL();
    }

    remove(fileName.c_str());

    LoggingFacility::clear();
}

}
}
}
