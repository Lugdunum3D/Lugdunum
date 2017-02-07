#include <lug/Window/Android/WindowImplAndroid.hpp>

namespace lug {
namespace Window {
namespace priv {

std::queue<lug::Window::Event> lug::Window::priv::WindowImpl::events;
AInputQueue* lug::Window::priv::WindowImpl::inputQueue = nullptr;
ANativeWindow* lug::Window::priv::WindowImpl::nativeWindow = nullptr;
ANativeActivity* lug::Window::priv::WindowImpl::activity = nullptr;


WindowImpl::WindowImpl(Window* win): _parent{win} {}

bool WindowImpl::init(const Window::InitInfo&) {

            _parent->_mode.width = 1920;
            _parent->_mode.height = 1080;
    return true;
}

void WindowImpl::close() {}

ANativeWindow* WindowImpl::getWindow() {
    return nativeWindow;
}

bool WindowImpl::pollEvent(lug::Window::Event& event) {

    if (inputQueue != nullptr) {
        AInputEvent *androidEvent = nullptr;
        while (AInputQueue_getEvent(inputQueue, &androidEvent) >= 0) {
            if (AInputQueue_preDispatchEvent(inputQueue, androidEvent)) {
                continue;
            }

            AInputQueue_finishEvent(inputQueue, androidEvent, 0);
        }
    }

    if (!events.empty()) {
        event = events.front();
        events.pop();
        return true;
    }

    return false;
}

} // namespace priv
} // namespace Window
} // namespace lug
