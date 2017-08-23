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

// credits to nvidia : https://github.com/NVIDIAGameWorks/GraphicsSamples/blob/master/extensions/src/NvGamepad/android/NvGamepadAndroid.cpp
float WindowImpl::MapCenteredAxis(AInputEvent* event, int32_t axis) {
    const float deadZone = (8689.0f / 32768.0f); // 0,2651672363
    float value = AMotionEvent_getAxisValue(event, axis, 0);
    if (value > deadZone) {
        return (value - deadZone) / (1.0f - deadZone);
    } else if (value < -deadZone) {
        return (value + deadZone) / (1.0f - deadZone);
    } else {
        return 0.0f;
    }
}

bool WindowImpl::pollEvent(lug::Window::Event& event) {

    if (inputQueue != nullptr) {
        AInputEvent* androidEvent = nullptr;

        if (AInputQueue_getEvent(inputQueue, &androidEvent) >= 0) {
            if (AInputQueue_preDispatchEvent(inputQueue, androidEvent)) {
                return false;
            }

            if (AInputEvent_getType(androidEvent) == AINPUT_EVENT_TYPE_MOTION) {

                if (AInputEvent_getSource(androidEvent) == AINPUT_SOURCE_JOYSTICK) {
                    // Left thumbstick
                    float axisLeftX = MapCenteredAxis(androidEvent, AMOTION_EVENT_AXIS_X);
                    float axisLeftY = MapCenteredAxis(androidEvent, AMOTION_EVENT_AXIS_Y);

                    // // Right thumbstick
                    float axisRightX = MapCenteredAxis(androidEvent, AMOTION_EVENT_AXIS_Z);
                    float axisRightY = MapCenteredAxis(androidEvent, AMOTION_EVENT_AXIS_RZ);

                    event.type = Event::Type::GamePadChange;
                    event.gamePad.axisLeft = {axisLeftX, axisLeftY};
                    event.gamePad.axisRight = {axisRightX, axisRightY};

                    events.push(std::move(event));
                }

                if (AInputEvent_getSource(androidEvent) == AINPUT_SOURCE_TOUCHSCREEN) {
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
                }
            }
            if (AInputEvent_getType(androidEvent) == AINPUT_EVENT_TYPE_KEY) {
                // TODO
            }

            AInputQueue_finishEvent(inputQueue, androidEvent, 1);
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
