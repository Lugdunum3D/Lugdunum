#include <lug/System/Logger.hpp>
#include <gtest/gtest.h>

#include "MockHandler.hpp"

namespace lug {
namespace System {

constexpr const char* loggerName = "MyTestLogger";
constexpr const char* handlerName = "MyTestHandler";
constexpr const char* handlerName2 = "MyTestHandler2";
constexpr const char* helloWorld = "Hello world!";

using namespace ::testing;


TEST(Logger, OneHandler) {
    Logger* logger = makeLogger(loggerName);
    MockHandler* handler = makeHandler<MockHandler>(handlerName);

    EXPECT_CALL(*handler, handle(AllOf(
        Field(&priv::Message::loggerName, StrEq(loggerName)),
        Field(&priv::Message::level, Level::Info),
        Field(&priv::Message::raw, Property(&fmt::MemoryWriter::c_str, StrEq(helloWorld)))
    ))).Times(1);

    logger->addHandler(handler);
    logger->info(helloWorld);
}


TEST(Logger, TwoHandlers) {
    Logger* logger = makeLogger(loggerName);
    MockHandler* handler = makeHandler<MockHandler>(handlerName);
    MockHandler* handler2 = makeHandler<MockHandler>(handlerName2);

    EXPECT_CALL(*handler, handle(AllOf(
        Field(&priv::Message::loggerName, StrEq(loggerName)),
        Field(&priv::Message::level, Level::Info),
        Field(&priv::Message::raw, Property(&fmt::MemoryWriter::c_str, StrEq(helloWorld)))
    ))).Times(1);

    EXPECT_CALL(*handler2, handle(AllOf(
        Field(&priv::Message::loggerName, StrEq(loggerName)),
        Field(&priv::Message::level, Level::Info),
        Field(&priv::Message::raw, Property(&fmt::MemoryWriter::c_str, StrEq(helloWorld)))
    ))).Times(1);

    logger->addHandler(handler);
    logger->addHandler(handler2);
    logger->info(helloWorld);
}


TEST(Logger, Fmt) {
    Logger* logger = makeLogger(loggerName);
    MockHandler* handler = makeHandler<MockHandler>(handlerName);

    EXPECT_CALL(*handler, handle(AllOf(
        Field(&priv::Message::loggerName, StrEq(loggerName)),
        Field(&priv::Message::level, Level::Info),
        Field(&priv::Message::raw, Property(&fmt::MemoryWriter::c_str, StrEq("abracadabra 101010")))
    ))).Times(1);

    logger->addHandler(handler);
    logger->info("{0}{1}{0} {2:b}", "abra", "cad", 42);
}


TEST(Logger, LevelsExplicit) {
    class MockLogger : public Logger {
    public:
        using Logger::Logger;
        MOCK_METHOD1(handle, void(priv::Message& msg));
    };

    std::unique_ptr<MockLogger> logger = std::make_unique<MockLogger>(loggerName);

    auto testOneLevel = [&](Level::enumLevel level) {
        EXPECT_CALL(*(logger.get()), handle(Field(&priv::Message::level, level))).Times(1);
        logger->log(level, helloWorld);
    };

    testOneLevel(Level::Off);
    testOneLevel(Level::Debug);
    testOneLevel(Level::Info);
    testOneLevel(Level::Warning);
    testOneLevel(Level::Error);
    testOneLevel(Level::Fatal);
    testOneLevel(Level::Assert);
}


TEST(Logger, LevelsImplicit) {
    class MockLogger : public Logger {
    public:
        using Logger::Logger;
        MOCK_METHOD1(handle, void(priv::Message& msg));
    };

    std::unique_ptr<MockLogger> logger = std::make_unique<MockLogger>(loggerName);

    // We can't make a lambda as earlier because we would have to pass functions like `logger->debug` as a
    // member pointer but it's not possible because they are templated.
    // You can try, go ahead, but it won't work.

    EXPECT_CALL(*(logger.get()), handle(Field(&priv::Message::level, Level::Debug))).Times(1);
    logger->debug(helloWorld);

    EXPECT_CALL(*(logger.get()), handle(Field(&priv::Message::level, Level::Info))).Times(1);
    logger->info(helloWorld);

    EXPECT_CALL(*(logger.get()), handle(Field(&priv::Message::level, Level::Warning))).Times(1);
    logger->warn(helloWorld);

    EXPECT_CALL(*(logger.get()), handle(Field(&priv::Message::level, Level::Error))).Times(1);
    logger->error(helloWorld);

    EXPECT_CALL(*(logger.get()), handle(Field(&priv::Message::level, Level::Fatal))).Times(1);
    logger->fatal(helloWorld);

    EXPECT_CALL(*(logger.get()), handle(Field(&priv::Message::level, Level::Assert))).Times(1);
    logger->assrt(helloWorld);
}


TEST(Logger, LevelsFilter) {
    Logger* logger = makeLogger(loggerName);
    MockHandler* handler = makeHandler<MockHandler>(handlerName);
    logger->addHandler(handler);

    auto testOneLevel = [&](Level::enumLevel level, Level::enumLevel levelPlusOne) {
        handler->setLevel(levelPlusOne);
        EXPECT_CALL(*handler, handle(_)).Times(0);
        logger->log(level, helloWorld);

        handler->setLevel(level);
        EXPECT_CALL(*handler, handle(_)).Times(1);
        logger->log(level, helloWorld);
    };

    testOneLevel(Level::Debug, Level::Info);
    testOneLevel(Level::Info, Level::Warning);
    testOneLevel(Level::Warning, Level::Error);
    testOneLevel(Level::Error, Level::Fatal);
    testOneLevel(Level::Fatal, Level::Assert);
    testOneLevel(Level::Assert, Level::Off);
}


TEST(Logger, Handlers) {
    class MockLogger : public Logger {
    public:
        using Logger::Logger;
        // MOCK_METHOD1(handle, void(priv::Message& msg));

        std::set<Handler*>& getHandlers() {
            return _handlers;
        }
    };

    std::unique_ptr<MockLogger> logger = std::make_unique<MockLogger>(loggerName);

    ASSERT_EQ(logger->getHandlers().size(), 0);

    MockHandler* handler = makeHandler<MockHandler>(handlerName);

    // Check that we can add a handler
    logger->addHandler(handler);
    ASSERT_THAT(logger->getHandlers(), UnorderedElementsAre(handler));

    // Check that we can't add the same handler twice
    logger->addHandler(handler);
    ASSERT_THAT(logger->getHandlers(), UnorderedElementsAre(handler));

    MockHandler* handler2 = makeHandler<MockHandler>(handlerName2);

    // Check that we can add a handler by name
    logger->addHandler(handlerName2);
    ASSERT_THAT(logger->getHandlers(), UnorderedElementsAre(handler, handler2));

    // Check that we can't add the same handler by name twice
    logger->addHandler(handlerName2);
    ASSERT_THAT(logger->getHandlers(), UnorderedElementsAre(handler, handler2));
}


namespace OstreamCustomClass {

class Custom {
public:
    std::ostream& operator<<(std::ostream& out) const;
    friend std::ostream& operator<<(std::ostream& out, const Custom& b) {
        return out << "Test";
    }
};

TEST(Logger, OstreamCustomClass) {

    Logger* logger = makeLogger(loggerName);
    MockHandler* handler = makeHandler<MockHandler>(handlerName);
    Custom custom;

    EXPECT_CALL(*handler, handle(Field(&priv::Message::raw, Property(&fmt::MemoryWriter::c_str, StrEq("Test"))))).Times(2);

    logger->addHandler(handler);
    logger->info("{}", custom);
    logger->info(custom);
}

} // namespace OstreamCustomClass


namespace ExceptionHandler {

class Custom {
public:
    std::ostream& operator<<(std::ostream& out) const;
    friend std::ostream& operator<<(std::ostream& out, const Custom& b) {
        throw std::runtime_error("Uncaught");
        return out;
    }
};

TEST(Logger, ExceptionHandler) {

    Logger* logger = makeLogger(loggerName);
    MockHandler* handler = makeHandler<MockHandler>(handlerName);
    Custom custom;

    EXPECT_CALL(*handler, handle(Field(&priv::Message::raw, Property(&fmt::MemoryWriter::c_str, HasSubstr("Uncaught"))))).Times(1);

    logger->addHandler(handler);
    logger->info(custom);
}

} // namespace ExceptionHandler


TEST(Logger, TimePattern) {
    // TODO
}


}
}

