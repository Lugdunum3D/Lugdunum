#include <lug/Core/Application.hpp>

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
    while (!_closed && _window && _window->isOpen()) {
        // Poll events
        {
            lug::Window::Event event;
            while (_window->pollEvent(event)) {
                onEvent(event);
            }
        }

        beginFrame();
        onFrame();
        endFrame();
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
