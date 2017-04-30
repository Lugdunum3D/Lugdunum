#include <lug/Graphics/Renderer.hpp>

namespace lug {
namespace Graphics {

Renderer::Renderer(Graphics& graphics, Renderer::Type type) : _graphics{graphics}, _type{type} {}

} // Graphics
} // lug
