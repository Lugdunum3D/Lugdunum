#include <lug/Window/Android/WindowImplAndroid.hpp>
#include <mutex>

namespace lug {
namespace Window {
namespace priv {

std::mutex WindowImpl::androidMutex;
std::condition_variable WindowImpl::cv;
std::queue<lug::Window::Event> WindowImpl::events;
AInputQueue* WindowImpl::inputQueue = nullptr;
ANativeWindow* WindowImpl::nativeWindow = nullptr;
ANativeActivity* WindowImpl::activity = nullptr;

WindowImpl::WindowImpl(Window* win) : _parent{win} {}

bool WindowImpl::init(const Window::InitInfo&) {
    if (!WindowImpl::nativeWindow) {
        std::unique_lock<std::mutex> lk(WindowImpl::androidMutex);
        WindowImpl::cv.wait(lk);
    }
    _parent->_mode.width = ANativeWindow_getWidth(nativeWindow);
    _parent->_mode.height = ANativeWindow_getHeight(nativeWindow);
    return true;
}

void WindowImpl::close() {}

ANativeWindow* WindowImpl::getWindow() {
    return nativeWindow;
}

bool WindowImpl::pollEvent(lug::Window::Event& event) {

    if (inputQueue != nullptr) {
        AInputEvent* androidEvent = nullptr;

        while (AInputQueue_getEvent(inputQueue, &androidEvent) >= 0) {
            if (AInputQueue_preDispatchEvent(inputQueue, androidEvent)) {
                continue;
            }

            AInputQueue_finishEvent(inputQueue, androidEvent, 1);
        }
    }

    if (!events.empty()) {
        event = events.front();
        events.pop();

        if (event.type == Event::Type::Resize) {
            _parent->_mode.width = ANativeWindow_getWidth(nativeWindow);
            _parent->_mode.height = ANativeWindow_getHeight(nativeWindow);            
        }
        // if (event.type == Event::Type::AndroidDestroy) {
        //     _window
        // }

        return true;
    }

    return false;
}

void WindowImpl::setKeyRepeat(bool state) {
    (void)state;
    // TODO
}

void WindowImpl::setMouseCursorVisible(bool visible) {
    (void)visible;
    // TODO
}

void WindowImpl::setMousePos(const Math::Vec2i& mousePosition) {
    (void)mousePosition;
    // TODO
}

} // namespace priv
} // namespace Window
} // namespace lug
