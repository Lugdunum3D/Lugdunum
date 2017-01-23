#include <lug/System/Logger.hpp>
#include <lug/System/Logger/OstreamHandler.hpp>

#include "Application.hpp"

int main(int argc, char *argv[]) {
    lug::System::Logger::logger.addHandler(lug::System::makeHandler<lug::System::StdoutHandler>("stdout"));

    Application app;

    if (!app.init(argc, argv)) {
        return 1;
    }

    return app.run() ? 0 : 1;
}
