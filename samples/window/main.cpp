#include <lug/Window/Window.hpp>
#include <lug/System/Logger/Logger.hpp>

#if defined(LUG_SYSTEM_ANDROID)
    #include <lug/System/Logger/LogCatHandler.hpp>
#else
    #include <lug/System/Logger/OstreamHandler.hpp>
#endif

int main() {
//    auto window = lug::Window::Window::create(800, 600, "Default Window", lug::Window::Style::Default);
    auto window = lug::Window::Window::create();


    auto logger = lug::System::makeLogger("LugdunumSamples");

#if defined(LUG_SYSTEM_ANDROID)
    auto handler = lug::System::makeHandler<lug::System::LogCatHandler>("LogCat");
#else
    auto handler = lug::System::makeHandler<lug::System::StdoutHandler>("Stdout");
#endif

    logger->addHandler(handler);

    if (!window) {
        logger->fatal("Window was not created");
        return 1;
    }

    logger->info("Window was created successfully");

    // While window is open execute the following
    while (window->isOpen()) {
        lug::Window::Event event;
        while (window->pollEvent(event)) {
            logger->info("Event received: {}", static_cast<uint32_t>(event.type));

            if (event.type == lug::Window::EventType::CLOSE) {
                logger->info("Closing the app");
                window->close();
            }
        }

        // ...
    }

    return 0;
}
