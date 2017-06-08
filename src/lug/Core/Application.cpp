#include <chrono>
#include <lug/Core/Application.hpp>
#include <lug/System/Clock.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Core {

Application::Application(const Application::Info& info) : _info{info}, _graphics{info.name, info.version} {}

bool Application::init(int argc, char* argv[]) {
    return beginInit(argc, argv) && finishInit();
}

bool Application::beginInit(int argc, char* argv[]) {
    // TODO: Use argc and argv to parse the command line
    (void)(argc);
    (void)(argv);

    if (!_graphics.beginInit(_graphicsInitInfo)) {
        return false;
    }

    return true;
}

bool Application::finishInit() {
    if (!_graphics.finishInit()) {
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
        const auto elapsedTime = clock.reset();

        // Poll events
        {
            lug::Window::Event event;

            while (_window->pollEvent(event)) {
                onEvent(event);
            }
        }

        beginFrame(elapsedTime);
        onFrame(elapsedTime);
        endFrame();

        elapsed += elapsedTime.getSeconds<float>();
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

bool Application::beginFrame(const lug::System::Time &elapsedTime) {
    return _graphics.getRenderer()->beginFrame(elapsedTime);
}

bool Application::endFrame() {
    return _graphics.getRenderer()->endFrame();
}

} // Core
} // lug
