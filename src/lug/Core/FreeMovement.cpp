#include <lug/Core/FreeMovement.hpp>
#include <lug/System/Logger/Logger.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

namespace lug {
namespace Core {

void FreeMovement::onFrame(const System::Time& elapsedTime) {
    if (_eventSource == nullptr) {
        LUG_LOG.warn("FreeMovement::onFrame: No event source, call setEventSource()");
        return;
    }

    // Move Forward
    if (_eventSource->isKeyPressed(lug::Window::Keyboard::Key::S)) {
        _target->translate({0.0f, 0.0f, _speed * elapsedTime.getMilliseconds<float>()});
    }
    // Move Backward
    if (_eventSource->isKeyPressed(lug::Window::Keyboard::Key::Z)) {
        _target->translate({0.0f, 0.0f, -_speed * elapsedTime.getMilliseconds<float>()});
    }
    // Strafe Left
    if (_eventSource->isKeyPressed(lug::Window::Keyboard::Key::Q)) {
        _target->translate({-_speed * elapsedTime.getMilliseconds<float>(), 0.0f, 0.0f});
    }
    // Strafe Right
    if (_eventSource->isKeyPressed(lug::Window::Keyboard::Key::D)) {
        _target->translate({_speed * elapsedTime.getMilliseconds<float>(), 0.0f, 0.0f});
    }

    // Move Upward
    if (_eventSource->isKeyPressed(lug::Window::Keyboard::Key::Space)) {
        _target->translate({0.0f, _speed * elapsedTime.getMilliseconds<float>(), 0.0f});
    }
    // Move Downward
    if (_eventSource->isKeyPressed(lug::Window::Keyboard::Key::LShift)) {
        _target->translate({0.0f, -_speed * elapsedTime.getMilliseconds<float>(), 0.0f});
    }


    // Capture / Release the mouse cursor
    if (_eventSource->isKeyPressed(lug::Window::Keyboard::Key::C) && !_hasFocus) {
        _lastMousePos = _eventSource->getMousePos();
        _hasFocus = true;
        _eventSource->setMouseCursorVisible(false);
    }
    if (_eventSource->isKeyPressed(lug::Window::Keyboard::Key::LAlt) && _hasFocus) {
        _hasFocus = false;
        _eventSource->setMouseCursorVisible(true);
    }

    if (_hasFocus) {
        auto mousePos = _eventSource->getMousePos();

        // Only if the mouse moved since the last time
        if (_lastMousePos != mousePos) {
            Math::Vec2i delta = mousePos - _lastMousePos;
            _lastMousePos = mousePos;

            // Rotate in world space to freeze the rotation on the X axis
            _target->rotate(_speed * 0.4f * -delta.x(), {0, 1, 0}, lug::Graphics::Node::TransformSpace::World);
            _target->rotate(_speed * 0.4f * -delta.y(), {1, 0, 0});

            Math::Vec2i windowSize = _eventSource->getWindowSize();

            // If the mouse escapes a 30% zone on the edges of the screen, we reset-it on the middle
            if (mousePos.x() < windowSize.width() * 0.30 || mousePos.x() > windowSize.width() * 0.70
                || mousePos.y() < windowSize.height() * 0.30 || mousePos.y() > windowSize.height() * 0.70) {
                Math::Vec2i middle{windowSize.width() / 2, windowSize.height() / 2};
                _eventSource->setMousePos(middle);
                _lastMousePos = middle;
            }
        }
    }
}

}
}

