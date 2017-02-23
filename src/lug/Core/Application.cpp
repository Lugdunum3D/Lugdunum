#include <chrono>
#include <lug/Core/Application.hpp>
#include <lug/System/Clock.hpp>
#include <lug/System/Logger/Logger.hpp>

#include <android/log.h>
namespace lug {
namespace Core {

Application::Application(const Application::Info& info) : _info{info}, _graphics{info.name, info.version} {
     LUG_LOG.error("Application OK");
    __android_log_print(ANDROID_LOG_VERBOSE, "lugdu", "APPLI OK");

}

bool Application::init(int argc, char* argv[]) {
    return beginInit(argc, argv) && finishInit();
}

bool Application::beginInit(int argc, char* argv[]) {
    // TODO: Use argc and argv to parse the command line
    (void)(argc);
    (void)(argv);

    if (!_graphics.init(_graphicsInitInfo)) {
        return false;
    }

    return true;
}

bool Application::finishInit() {
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

        beginFrame();
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

bool Application::beginFrame() {
    return _graphics.getRenderer()->beginFrame();
}

bool Application::endFrame() {
    return _graphics.getRenderer()->endFrame();
}

} // Core
} // lug
