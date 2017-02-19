#include <chrono>
#include <lug/Core/Application.hpp>
#include <lug/System/Clock.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Core {

Application::Application(const Application::Info& info) : _info{info}, _graphics{info.name, info.version} {}

bool Application::init(int argc, char* argv[]) {
    // TODO: Use argc and argv to parse the command line
    (void)(argc);
    (void)(argv);

    if (!_graphics.init(_graphicsInitInfo)) {
        return false;
    }

    _window = _graphics.getRenderer()->createWindow(_renderWindowInitInfo);
    if (!_window) {
        return false;
    }

    return true;
}

bool Application::run() {
    float elapsed = 0;
    uint32_t frames = 0;
    System::Clock clock;
    while (!_closed && _window && _window->isOpen()) {
        const auto elapsedTime = clock.getElapsedTime();
        clock.reset();

        // Poll events
        {
            lug::Window::Event event;
            while (_window->pollEvent(event)) {
                onEvent(event);
            }
        }

        beginFrame();
        onFrame(elapsedTime);
        endFrame();

        elapsed += elapsedTime.getSeconds();
        frames++;

        if (elapsed >= 1.0f) {
            LUG_LOG.info("FPS: {}", frames / elapsed);
            frames = 0;
            elapsed = 0;
        }
    }

    return true;
}

void Application::close() {
    _closed = true;
}

bool Application::beginFrame() {
    return _graphics.getRenderer()->beginFrame();
}

bool Application::endFrame() {
    return _graphics.getRenderer()->endFrame();
}

} // Core
} // lug
