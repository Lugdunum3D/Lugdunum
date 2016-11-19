#include <lug/System/Logger.hpp>
#include <gtest/gtest.h>

class Util
{
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


TEST(Logger, OneParameter) {
    auto logger = lug::System::makeStdoutLogger("myLogger");
    Util::captureStdout();
    logger->info("Hello world!");
    std::string output = Util::getCaptured();
    EXPECT_EQ(output, "Hello world!\n");
    Util::stopCapture();
}

TEST(Logger, Fmt) {
    auto logger = lug::System::makeStdoutLogger("myLogger");
    Util::captureStdout();
    logger->info("Hello {}!", "world");
    std::string output = Util::getCaptured();
    EXPECT_EQ(output, "Hello world!\n");
    Util::stopCapture();
}
