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
    ndk_helper::GESTURE_STATE doubleTapState = doubletap_detector_.Detect(androidEvent);
    ndk_helper::GESTURE_STATE dragState = drag_detector_.Detect(androidEvent);
    ndk_helper::GESTURE_STATE pinchState = pinch_detector_.Detect(androidEvent);

    // Double tap detector has a priority over other detectors
    if (doubleTapState == ndk_helper::GESTURE_STATE_ACTION) {
      // Detect double tap
        LUG_LOG.info("should start detecting double tap");
        
    } else {
      // Handle drag state
      if (dragState & ndk_helper::GESTURE_STATE_START) {
        
        // // Otherwise, start dragging
        lug::Math::Vec2f v;
         drag_detector_.GetPointer(v);
        // TransformPosition(v);
        // tap_camera_.BeginDrag(v);
        event.touchScreen.coordinates[0] = v;      
        LUG_LOG.info("start dragging");
      } else if (dragState & ndk_helper::GESTURE_STATE_MOVE) {
        event.touchScreen.drag = true;
        lug::Math::Vec2f v;
        drag_detector_.GetPointer(v);
        event.touchScreen.coordinates[0] = v;
        // ndk_helper::Vec2 v;
        // drag_detector_.GetPointer(v);
        // TransformPosition(v);
        // tap_camera_.Drag(v);
        LUG_LOG.info("dragging");
      } else if (dragState & ndk_helper::GESTURE_STATE_END) {
        LUG_LOG.info("end dragging");
        // tap_camera_.EndDrag();
      }

      // Handle pinch state
      if (pinchState & ndk_helper::GESTURE_STATE_START) {
        LUG_LOG.info("Start new pinch");
        event.touchScreen.pinch = true;

  
        pinch_detector_.GetPointers(event.touchScreen.coordinates[0], event.touchScreen.coordinates[1]);


        // // Start new pinch
        // ndk_helper::Vec2 v1;
        // ndk_helper::Vec2 v2;
        // pinch_detector_.GetPointers(v1, v2);
        // TransformPosition(v1);
        // TransformPosition(v2);
        // tap_camera_.BeginPinch(v1, v2);
      } else if (pinchState & ndk_helper::GESTURE_STATE_MOVE) {
        // Multi touch
        // Start new pinch
        LUG_LOG.info("Multi touch  Start new pinch");
        event.touchScreen.pinch = true;
        
        pinch_detector_.GetPointers(event.touchScreen.coordinates[0], event.touchScreen.coordinates[1]);
                   
        // ndk_helper::Vec2 v1;
        // ndk_helper::Vec2 v2;
        // pinch_detector_.GetPointers(v1, v2);
        // TransformPosition(v1);
        // TransformPosition(v2);
        // tap_camera_.Pinch(v1, v2);
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

            // if (AInputEvent_getType(androidEvent) == AINPUT_EVENT_TYPE_MOTION) {

            //     if (AInputEvent_getSource(androidEvent) == AINPUT_SOURCE_JOYSTICK) {
            //         // Left thumbstick
            //         float axisLeftX = MapCenteredAxis(androidEvent, AMOTION_EVENT_AXIS_X);
            //         float axisLeftY = MapCenteredAxis(androidEvent, AMOTION_EVENT_AXIS_Y);

            //         // // Right thumbstick
            //         float axisRightX = MapCenteredAxis(androidEvent, AMOTION_EVENT_AXIS_Z);
            //         float axisRightY = MapCenteredAxis(androidEvent, AMOTION_EVENT_AXIS_RZ);

            //         event.type = Event::Type::GamePadChange;
            //         event.gamePad.axisLeft = {axisLeftX, axisLeftY};
            //         event.gamePad.axisRight = {axisRightX, axisRightY};

            //         events.push(std::move(event));
            //     }

            //     if (AInputEvent_getSource(androidEvent) == AINPUT_SOURCE_TOUCHSCREEN) {
            //         event.mouse.code = Mouse::Button::Left;
            //         event.mouse.coord.x = AMotionEvent_getX(androidEvent, 0);
            //         event.mouse.coord.y = AMotionEvent_getY(androidEvent, 0);
            //         switch (AKeyEvent_getAction(androidEvent)) {
            //             case AKEY_EVENT_ACTION_DOWN:
            //                 event.type = Event::Type::ButtonPressed;
            //             break;

            //             case AKEY_EVENT_ACTION_UP:
            //                 event.type = Event::Type::ButtonReleased;
            //             break;

            //             default:
            //             break;
            //         }
            //     }
            // }
            // if (AInputEvent_getType(androidEvent) == AINPUT_EVENT_TYPE_KEY) {
            //     // TODO
            // }

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
