#include <lug/Graphics/Graphics.hpp>
#include <lug/Window/Window.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>
#include <lug/System/Logger.hpp>
#include <lug/System/Logger/OstreamHandler.hpp>

int main() {
    auto logger = lug::System::makeLogger("myLogger");
    logger->addHandler(lug::System::makeHandler<lug::System::StdoutHandler>("myLoggerStdout"));

    lug::Graphics::Graphics graphics{{"hello", {0, 1, 0}}};

    // graphics.setRendererType(Renderer::Type::Vulkan);
    // graphics.addModule(...);

    // Add or remove modules
    // Set the renderer type

    graphics.init();

    lug::Graphics::Renderer* renderer = graphics.getRenderer();

    auto window = renderer->createWindow(800, 600, "Default Window", lug::Window::Style::Default);

    if (!window->init()) {
        logger->error("Failed to init window");
        return 1;
    }

    while (window->isOpen()) {
        lug::Window::Event event;
        while (window->pollEvent(event)) {
            if (event.type == lug::Window::EventType::CLOSE) {
                logger->info("Closing the app");
                window->close();
            }
        }
    }

    return 0;
}
