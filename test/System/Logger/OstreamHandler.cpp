#include <lug/System/Logger/OstreamHandler.hpp>
#include <lug/System/Logger/Message.hpp>
#include <lug/System/Logger/Formatter.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace lug {
namespace System {

using namespace ::testing;

namespace {

class Util {
public:
    static void capture(std::ostream& capture) {
        _orig = &capture;

        // Save cout's buffer here
        _sbuf = _orig->rdbuf();

        clearBuffer();

        // Redirect cout to our stringstream buffer or any other ostream
        _orig->rdbuf(_buffer.rdbuf());
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
        _orig->rdbuf(_sbuf);
        _orig = nullptr;
        _sbuf = nullptr;
        clearBuffer();
    }

private:
    static std::stringstream _buffer;
    static std::streambuf* _sbuf;
    static std::ostream* _orig;
};

std::stringstream  Util::_buffer{};
std::streambuf* Util::_sbuf{};
std::ostream* Util::_orig{};

} // namespace {}

constexpr const char* handlerName = "MyTestHandler";

TEST(OstreamHandler, Stdout) {
    Handler* handler = makeHandler<OstreamHandler>(handlerName, std::cout);
    priv::Message msg;
    msg.formatted << "Hello world!";
    Util::capture(std::cout);
    handler->handle(msg);
    EXPECT_EQ(Util::getCaptured(), "Hello world!");
    Util::stopCapture();

    LoggingFacility::clear();
}

TEST(OstreamHandler, Stderr) {
    Handler* handler = makeHandler<OstreamHandler>(handlerName, std::cerr);
    priv::Message msg;
    msg.formatted << "Hello world!";
    Util::capture(std::cerr);
    handler->handle(msg);
    EXPECT_EQ(Util::getCaptured(), "Hello world!");
    Util::stopCapture();

    LoggingFacility::clear();
}

TEST(OstreamHandler, StdoutHandler) {
    Handler* handler = makeHandler<StdoutHandler>(handlerName);
    priv::Message msg;
    msg.formatted << "Hello world!";
    Util::capture(std::cout);
    handler->handle(msg);
    EXPECT_EQ(Util::getCaptured(), "Hello world!");
    Util::stopCapture();

    LoggingFacility::clear();
}

TEST(OstreamHandler, StderrHandler) {
    Handler* handler = makeHandler<StderrHandler>(handlerName);
    priv::Message msg;
    msg.formatted << "Hello world!";
    Util::capture(std::cerr);
    handler->handle(msg);
    EXPECT_EQ(Util::getCaptured(), "Hello world!");
    Util::stopCapture();

    LoggingFacility::clear();
}

TEST(OstreamHandler, StringStream) {
    std::stringstream test;

    Handler* handler = makeHandler<OstreamHandler>(handlerName, test);
    priv::Message msg;
    msg.formatted << "Hello world!";
    Util::capture(test);
    handler->handle(msg);
    EXPECT_EQ(Util::getCaptured(), "Hello world!");
    Util::stopCapture();

    LoggingFacility::clear();
}

TEST(OstreamHandler, CallsFormat) {
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

    Handler* handler = makeHandler<StdoutHandler>(handlerName);
    handler->setFormatter(std::move(formatter));
    priv::Message msg("Test", Level::Info);
    msg.raw << "Hello world!";
    handler->format(msg);

    LoggingFacility::clear();
}


}
}
