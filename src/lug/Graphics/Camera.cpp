#include <lug/Graphics/Camera.hpp>

namespace lug {
namespace Graphics {

Camera::Camera(const std::string& name) : Node(name), _fov(45.0f) {}

} // Graphics
} // lug
