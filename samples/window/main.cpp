#include <lug/Window/Window.hpp>
#include <lug/System/Logger.hpp>
#include <lug/System/Logger/LogCatHandler.hpp>

int main() {
    auto window = lug::Window::Window::create(800, 600, "Default Window", lug::Window::Style::Default);

    auto logger = lug::System::makeLogger("LugdunumSamples");
    auto handler = lug::System::makeHandler<lug::System::LogCatHandler>("LogCat");

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
