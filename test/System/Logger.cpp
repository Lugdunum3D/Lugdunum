#include <lug/System/Logger.hpp>
#include <lug/System/Logger/StdoutHandler.hpp>
#include <gtest/gtest.h>

class Util {
public:
    static void captureStdout() {
        // Save cout's buffer here
        _sbuf = std::cout.rdbuf();

        clearBuffer();

        // Redirect cout to our stringstream buffer or any other ostream
        std::cout.rdbuf(_buffer.rdbuf());
    }

    static std::string getCaptured(bool stop = false) {
        std::string captured = _buffer.str();
        if (stop) {
            stopCapture();
        }
        else {
            clearBuffer();
        }
        return captured;
    }

    static void clearBuffer() {
        _buffer.str("");
        _buffer.clear();
    }

    static void stopCapture() {
        // When done redirect cout to its old self
        std::cout.rdbuf(_sbuf);
        _sbuf = nullptr;
        clearBuffer();
    }

private:
    static std::stringstream _buffer;
    static std::streambuf* _sbuf;
};

std::stringstream  Util::_buffer{};
std::streambuf* Util::_sbuf{};


TEST(LoggerHandler, NoHandlers) {
    lug::System::Logger* logger = lug::System::makeLogger("MyTestLogger");
    Util::captureStdout();
    logger->info("Hello world!");
    std::string output = Util::getCaptured();
    EXPECT_EQ(output, "");
    Util::stopCapture();
}

TEST(LoggerHandler, OneHandler) {
    lug::System::Logger* logger = lug::System::makeLogger("MyTestLogger");
    lug::System::Handler* handler = lug::System::makeHandler<lug::System::StdoutHandler>("MyTestHandler");
    logger->addHandler(handler);
    Util::captureStdout();
    logger->info("Hello world!");
    std::string output = Util::getCaptured();
    EXPECT_EQ(output, "Hello world!\n");
    Util::stopCapture();
}

TEST(LoggerHandler, OneHandlerByName) {
    lug::System::Logger* logger = lug::System::makeLogger("MyTestLogger");
    lug::System::makeHandler<lug::System::StdoutHandler>("MyTestHandler");
    logger->addHandler("MyTestHandler");
    Util::captureStdout();
    logger->info("Hello world!");
    std::string output = Util::getCaptured();
    EXPECT_EQ(output, "Hello world!\n");
    Util::stopCapture();
}

TEST(LoggerHandler, TwoHandlersByPtr) {
    lug::System::Logger* logger = lug::System::makeLogger("MyTestLogger");
    lug::System::Handler* handler1 = lug::System::makeHandler<lug::System::StdoutHandler>("MyTestHandler1");
    lug::System::Handler* handler2 = lug::System::makeHandler<lug::System::StdoutHandler>("MyTestHandler2");
    logger->addHandler(handler1);
    logger->addHandler(handler2);
    Util::captureStdout();
    logger->info("Hello world!");
    std::string output = Util::getCaptured();
    EXPECT_EQ(output, "Hello world!\nHello world!\n");
    Util::stopCapture();
}

TEST(LoggerHandler, TwoHandlersByName) {
    lug::System::Logger* logger = lug::System::makeLogger("MyTestLogger");
    lug::System::makeHandler<lug::System::StdoutHandler>("MyTestHandler1");
    lug::System::makeHandler<lug::System::StdoutHandler>("MyTestHandler2");
    logger->addHandler("MyTestHandler1");
    logger->addHandler("MyTestHandler2");
    Util::captureStdout();
    logger->info("Hello world!");
    std::string output = Util::getCaptured();
    EXPECT_EQ(output, "Hello world!\nHello world!\n");
    Util::stopCapture();
}

TEST(Logger, Fmt) {
    lug::System::Logger* logger = lug::System::makeLogger("myLogger");
    lug::System::Handler* handler = lug::System::makeHandler<lug::System::StdoutHandler>("MyTestHandler");
    logger->addHandler(handler);
    Util::captureStdout();
    logger->info("Hello {}!", "world");
    std::string output = Util::getCaptured();
    EXPECT_EQ(output, "Hello world!\n");
    Util::stopCapture();
}

TEST(Logger, BasicPattern) {
    lug::System::Logger* logger = lug::System::makeLogger("myLogger");
    lug::System::Handler* handler = lug::System::makeHandler<lug::System::StdoutHandler>("MyTestHandler");
    logger->addHandler(handler);
    Util::captureStdout();
    logger->setPattern("%v\n");
    logger->info("Hello world!");
    std::string output = Util::getCaptured();
    EXPECT_EQ(output, "Hello world!\n");
    Util::stopCapture();
}

TEST(Logger, MediumPattern) {
    lug::System::Logger* logger = lug::System::makeLogger("myLogger");
    lug::System::Handler* handler = lug::System::makeHandler<lug::System::StdoutHandler>("MyTestHandler");
    logger->addHandler(handler);
    Util::captureStdout();
    logger->setPattern("[%v]\n");
    logger->info("Hello world!");
    std::string output = Util::getCaptured();
    EXPECT_EQ(output, "[Hello world!]\n");
    Util::stopCapture();
}

TEST(Logger, TimePattern) {
    lug::System::Logger* logger = lug::System::makeLogger("myLogger");
    lug::System::Handler* handler = lug::System::makeHandler<lug::System::StdoutHandler>("MyTestHandler");
    logger->addHandler(handler);
    Util::captureStdout();
    logger->setPattern("[%v]\n");
    logger->info("Hello world!");
    std::string output = Util::getCaptured();
    EXPECT_EQ(output, "[Hello world!]\n");
    Util::stopCapture();
}
