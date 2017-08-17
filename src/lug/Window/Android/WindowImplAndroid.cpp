#include <lug/Window/Android/WindowImplAndroid.hpp>

namespace lug {
namespace Window {
namespace priv {

std::queue<lug::Window::Event> WindowImpl::events;
AInputQueue* WindowImpl::inputQueue = nullptr;
ANativeWindow* WindowImpl::nativeWindow = nullptr;
ANativeActivity* WindowImpl::activity = nullptr;

WindowImpl::WindowImpl(Window* win) : _parent{win} {}

bool WindowImpl::init(const Window::InitInfo&) {
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
          
            switch (AInputEvent_getType(androidEvent) ) {
                case AINPUT_EVENT_TYPE_MOTION:
                    event.mouse.code = Mouse::Button::Left;
                    event.mouse.coord.x = AMotionEvent_getX(androidEvent, 0);
                    event.mouse.coord.y = AMotionEvent_getY(androidEvent, 0);
                    switch (AKeyEvent_getAction(androidEvent)) {
                        case AKEY_EVENT_ACTION_DOWN:
                            event.type = Event::Type::ButtonPressed;
                        break;

                        case AKEY_EVENT_ACTION_UP:
                            event.type = Event::Type::ButtonReleased;
                        break;

                        default:
                        break;
                }
                break;

                default:
                break;
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
