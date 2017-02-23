#include <lug/System/Logger/Logger.hpp>
#include <lug/System/Logger/OstreamHandler.hpp>

#include "Application.hpp"

int main(int argc, char *argv[]) {
    LUG_LOG.addHandler(lug::System::Logger::makeHandler<lug::System::Logger::StdoutHandler>("stdout"));

    Application app;

    if (!app.init(argc, argv)) {
        return 1;
    }

    return app.run() ? 0 : 1;
}
