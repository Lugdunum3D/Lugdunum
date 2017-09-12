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

int32_t WindowImpl::HandleInput(lug::Window::Event& event, AInputEvent* androidEvent) {
  // Engine* eng = (Engine*)app->userData;
  if (AInputEvent_getType(androidEvent) == AINPUT_EVENT_TYPE_MOTION) {
    event.type = Event::Type::TouchScreenChange;
    ndk_helper::GESTURE_STATE doubleTapState = doubletap_detector_.Detect(androidEvent);
    ndk_helper::GESTURE_STATE dragState = drag_detector_.Detect(androidEvent);
    ndk_helper::GESTURE_STATE pinchState = pinch_detector_.Detect(androidEvent);

    // Double tap detector has a priority over other detectors
    if (doubleTapState == ndk_helper::GESTURE_STATE_ACTION) {
        event.touchScreen.doubleTap = true;
    } else {
      // Handle drag state
      if (dragState & ndk_helper::GESTURE_STATE_START) {
        event.touchScreen.drag = false;
        event.touchScreen.tap= true;
        drag_detector_.GetPointer(event.touchScreen.coordinates[0]);
      } else if (dragState & ndk_helper::GESTURE_STATE_MOVE) {
        event.touchScreen.drag = true;
        event.touchScreen.tap= false;
        drag_detector_.GetPointer(event.touchScreen.coordinates[0]);
      } else if (dragState & ndk_helper::GESTURE_STATE_END) {
        event.touchScreen.drag = false;
        event.touchScreen.tap= false;
      }

      if (pinchState & (ndk_helper::GESTURE_STATE_START || ndk_helper::GESTURE_STATE_MOVE)) {
        event.touchScreen.pinch = true;
        pinch_detector_.GetPointers(event.touchScreen.coordinates[0], event.touchScreen.coordinates[1]);
      } else if (pinchState & ndk_helper::GESTURE_STATE_END) {
        event.touchScreen.pinch = false;

      }
    }
    return 1;
  }
  return 0;
}


bool WindowImpl::pollEvent(lug::Window::Event& event) {

    if (inputQueue != nullptr) {
        AInputEvent* androidEvent = nullptr;

        if (AInputQueue_getEvent(inputQueue, &androidEvent) >= 0) {
            if (AInputQueue_preDispatchEvent(inputQueue, androidEvent)) {
                return false;
            }

            HandleInput(event, androidEvent);
            LUG_LOG.info("WINDOWS ANDROID event coordinate  {} {}", event.touchScreen.coordinates[0].x(), event.touchScreen.coordinates[0].y());
            events.push(std::move(event));
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
