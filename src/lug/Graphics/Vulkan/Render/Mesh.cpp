#include <lug/Graphics/Vulkan/Render/Mesh.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

Mesh::Mesh(const std::string& name) : ::lug::Graphics::Render::Mesh(name) {}

Mesh::~Mesh() {
    destroy();
}

void Mesh::destroy() {
    for (auto& primitiveSet : _primitiveSets) {
        if (!primitiveSet._data) {
            continue;
        }

        PrimitiveSetData* primitiveSetData = static_cast<PrimitiveSetData*>(primitiveSet._data);

        for (auto& buffer : primitiveSetData->buffers) {
            buffer.destroy();
        }

        delete primitiveSetData;
    }

    _deviceMemory.destroy();
}

} // Render
} // Vulkan
} // Graphics
} // lug
