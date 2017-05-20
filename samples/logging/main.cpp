#include <lug/Config.hpp>
#include <lug/System/Exception.hpp>
#include <lug/System/Logger/Logger.hpp>
#include <lug/System/Logger/OstreamHandler.hpp>

int main(int, const char*[]) {
    auto logger = lug::System::Logger::makeLogger("myLogger");
    logger->addHandler(lug::System::Logger::makeHandler<lug::System::Logger::StdoutHandler>("myLoggerStdout"));

    logger->info("Application", "This will test logging!");

    logger->info("test", "Testing log muting...");
    logger->turnOff("test");
    logger->info("test", "If i'm correct, this shouldn't show");
    logger->turnOn("test");
    logger->info("test", "If i'm correct, this should show.");
    logger->muteLevel("test", lug::System::Logger::Level::Warning);
    logger->warn("test", "Oops!");
    logger->unmuteLevel("test", lug::System::Logger::Level::Warning);
    logger->warn("test", "Not Oops!");
    return 0;
}
