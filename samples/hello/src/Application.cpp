#include "Application.hpp"

Application::Application() : lug::Core::Application::Application{{"hello", {0, 1, 0}}} {
    getWindowInfo().title = "Hello Cube";
}

void Application::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::EventType::CLOSE) {
        close();
    }
}

void Application::onFrame() {
    // Do something on each frame
}
