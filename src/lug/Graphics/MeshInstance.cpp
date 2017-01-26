#include <lug/Graphics/MeshInstance.hpp>

namespace lug {
namespace Graphics {

MeshInstance::MeshInstance(const std::string& name) : MovableObject(name) {}

void MeshInstance::needUpdate() {
    // Do nothing here (called when the parent become dirty)
}

} // Graphics
} // lug
