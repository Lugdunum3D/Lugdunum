#include <lug/Graphics/Vulkan/Builder/Mesh.hpp>

#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Graphics.hpp>
#include <lug/Graphics/Render/Mesh.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Builder {

Mesh::Mesh(lug::Graphics::Renderer& renderer) : lug::Graphics::Builder::Mesh(renderer) {}

static std::unique_ptr<API::Buffer> createAttributeBuffer(
    Vulkan::Renderer* renderer,
    int& meshMemoryTypeIndex,
    VkDeviceSize& meshMemorySize,
    Render::Mesh::PrimitiveSet::Attribute& attribute) {

    auto& device = renderer->getDevice();

    VkBufferUsageFlags usage;
    if (attribute.type == lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Indice) {
        usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    }
    else {
        usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    }

    std::vector<uint32_t> queueFamilyIndices = { (uint32_t)renderer->getQueue(VK_QUEUE_GRAPHICS_BIT, false)->getFamilyIdx() };
    std::unique_ptr<API::Buffer> buffer = API::Buffer::create(&device,
        (uint32_t)queueFamilyIndices.size(),
        queueFamilyIndices.data(),
        attribute.buffer.size, usage);

    // Check the memory type of the buffer
    // All the buffers should have the same memory type
    // Because we create one device memory for all the buffers (So, with one type)
    auto& requirements = buffer->getRequirements();
    int attributeMemoryTypeIndex = API::DeviceMemory::findMemoryType(&device, requirements, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    if (attributeMemoryTypeIndex != meshMemoryTypeIndex &&
        meshMemoryTypeIndex != -1) {
        LUG_LOG.error("Vulkan::Mesh::build: Buffers don't have the same memory type");
        return nullptr;
    }
    else if (attributeMemoryTypeIndex == -1) {
        LUG_LOG.error("Vulkan::Mesh::build: Memory type not supported");
        return nullptr;
    }

    if (meshMemorySize % requirements.alignment) {
        meshMemorySize += requirements.alignment - meshMemorySize % requirements.alignment;
    }

    meshMemoryTypeIndex = attributeMemoryTypeIndex;
    meshMemorySize += requirements.size;

    return buffer;
}

Resource::SharedPtr<::lug::Graphics::Render::Mesh> Mesh::build() {
    // Constructor of Mesh is private, we can't use std::make_unique
    std::unique_ptr<Resource> resource{new Vulkan::Render::Mesh(_name)};
    Vulkan::Render::Mesh* mesh = static_cast<Vulkan::Render::Mesh*>(resource.get());
    VkDeviceSize meshMemorySize = 0;
    int meshMemoryTypeIndex = -1;

    Vulkan::Renderer* renderer = static_cast<Vulkan::Renderer*>(&_renderer);
    auto& device = renderer->getDevice();

    for (auto& builderPrimitiveSet : _primitiveSets) {
        Render::Mesh::PrimitiveSetData* primitiveSetData = new Render::Mesh::PrimitiveSetData();
        lug::Graphics::Render::Mesh::PrimitiveSet targetPrimitiveSet;

        targetPrimitiveSet.mode = builderPrimitiveSet.getMode();
        targetPrimitiveSet.material = builderPrimitiveSet.getMaterial();

        auto& builderAttributes = builderPrimitiveSet.getAttributes();
        uint32_t attributesNb = static_cast<uint32_t>(builderAttributes.size());
        targetPrimitiveSet.attributes.resize(attributesNb);

        for (uint32_t i = 0; i < attributesNb; ++i) {
            targetPrimitiveSet.attributes[i] = builderAttributes[i];

            // Pipeline::Handle::PrimitivePart support only 3 texture coordinates
            if (targetPrimitiveSet.attributes[i].type == lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::TexCoord &&
                targetPrimitiveSet.texCoords.size() == 3) {
                LUG_LOG.warn("Vulkan::Mesh::build: More than 3 texture coordinates, others will be ignored");
                continue;
            }

            switch (targetPrimitiveSet.attributes[i].type) {
                case lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Indice:
                    targetPrimitiveSet.indices = &targetPrimitiveSet.attributes[i];
                    break;
                case lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Position:
                    targetPrimitiveSet.position = &targetPrimitiveSet.attributes[i];
                    break;
                case lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Normal:
                    targetPrimitiveSet.normal = &targetPrimitiveSet.attributes[i];
                    break;
                case lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::TexCoord:
                    targetPrimitiveSet.texCoords.push_back(&targetPrimitiveSet.attributes[i]);
                    break;
                case lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Tangent:
                    targetPrimitiveSet.tangent = &targetPrimitiveSet.attributes[i];
                    break;
            }

            std::unique_ptr<API::Buffer> buffer = createAttributeBuffer(
                renderer,
                meshMemoryTypeIndex,
                meshMemorySize,
                targetPrimitiveSet.attributes[i]);

            if (!buffer) {
                return nullptr;
            }

            primitiveSetData->buffers.push_back(std::move(buffer));
        }

        primitiveSetData->pipelineIdPrimitivePart.positionVertexData = targetPrimitiveSet.position != nullptr;
        primitiveSetData->pipelineIdPrimitivePart.normalVertexData = targetPrimitiveSet.normal != nullptr;
        primitiveSetData->pipelineIdPrimitivePart.tangentVertexData = targetPrimitiveSet.tangent != nullptr;
        primitiveSetData->pipelineIdPrimitivePart.countTexCoord = targetPrimitiveSet.texCoords.size();
        primitiveSetData->pipelineIdPrimitivePart.primitiveMode = static_cast<uint32_t>(targetPrimitiveSet.mode);

        targetPrimitiveSet._data = static_cast<void*>(primitiveSetData);
        mesh->_primitiveSets.push_back(targetPrimitiveSet);
    }

    mesh->_deviceMemory = API::DeviceMemory::allocate(&device, meshMemorySize, meshMemoryTypeIndex);

    // Bind attributes buffers to mesh device memory
    {
        VkDeviceSize deviceMemoryOffset = 0;
        for (auto& primitiveSet : mesh->_primitiveSets) {
            Render::Mesh::PrimitiveSetData* primitiveSetData = static_cast<Render::Mesh::PrimitiveSetData*>(primitiveSet._data);
            for (auto& buffer : primitiveSetData->buffers) {
                auto& requirements = buffer->getRequirements();

                if (deviceMemoryOffset % requirements.alignment) {
                    deviceMemoryOffset += requirements.alignment - deviceMemoryOffset % requirements.alignment;
                }

                buffer->bindMemory(mesh->_deviceMemory.get(), deviceMemoryOffset);
                deviceMemoryOffset += requirements.size;
            }
        }
    }

    return _renderer.getResourceManager()->add<::lug::Graphics::Render::Mesh>(std::move(resource));
}

} // Builder
} // Vulkan
} // Graphics
} // lug
