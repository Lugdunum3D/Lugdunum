#include <lug/Window/Android/WindowImplAndroid.hpp>

namespace lug {
namespace Window {
namespace priv {

lug::Window::priv::WindowImpl::WindowImpl(Window*) {}

bool lug::Window::priv::WindowImpl::create(const std::string&, Style) {
    return true;
}

void lug::Window::priv::WindowImpl::close() {}

bool lug::Window::priv::WindowImpl::pollEvent(lug::Window::Event& event) {

    if (lug::Main::inputQueue != nullptr) {
        AInputEvent *androidEvent = nullptr;
        while (AInputQueue_getEvent(lug::Main::inputQueue, &androidEvent) >= 0) {
            if (AInputQueue_preDispatchEvent(lug::Main::inputQueue, androidEvent)) {
                continue;
            }
            int32_t handled = 0;
            AInputQueue_finishEvent(lug::Main::inputQueue, androidEvent, handled);
        }
    }

    if (!lug::Main::events.empty()) {
        event = lug::Main::events.front();
        lug::Main::events.pop();
        return true;
    }

    return false;
}

} // namespace priv
} // namespace Window
} // namespace lug
