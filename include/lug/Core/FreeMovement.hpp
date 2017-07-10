#pragma once

#include <lug/Core/Export.hpp>
#include <lug/Graphics/Scene/Node.hpp>
#include <lug/Math/Vector.hpp>
#include <lug/Window/Window.hpp>
#include <lug/System/Time.hpp>

namespace lug {
namespace Core {

class LUG_CORE_API FreeMovement {
public:
    explicit FreeMovement() = default;
    ~FreeMovement() = default;

    /**
     * @brief      Sets the camera node (i.e. the node to which the camera is attached to).
     */
    void setTargetNode(lug::Graphics::Scene::Node& target);

    /**
     * @brief      Sets the event source, a Window instance.
     *
     * @param[in]  eventSource  The event source
     */
    void setEventSource(lug::Window::Window& eventSource);

    void onFrame(const System::Time& elapsedTime);

private:
    lug::Graphics::Scene::Node* _target{nullptr};
    lug::Window::Window* _eventSource{nullptr};
    float _speed{0.005f};

    bool _hasFocus{false};
    Math::Vec2i _lastMousePos{0, 0};
};

#include <lug/Core/FreeMovement.inl>

} // Core
} // lug

