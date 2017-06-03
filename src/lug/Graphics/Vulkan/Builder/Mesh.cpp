#include <lug/Graphics/Vulkan/Builder/Mesh.hpp>

#include <lug/Graphics/Builder/Mesh.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Graphics.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Builder {
namespace Mesh {

Resource::SharedPtr<::lug::Graphics::Render::Mesh> build(const ::lug::Graphics::Builder::Mesh& builder) {
    // Constructor of Mesh is private, we can't use std::make_unique
    std::unique_ptr<Resource> resource{new Vulkan::Render::Mesh(builder._name)};
    Vulkan::Render::Mesh* mesh = static_cast<Vulkan::Render::Mesh*>(resource.get());

    const Vulkan::Renderer& renderer = static_cast<Vulkan::Renderer&>(builder._renderer);

    for (auto& builderPrimitiveSet : builder._primitiveSets) {
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

            {
                API::Buffer buffer;
                API::Builder::Buffer bufferBuilder(renderer.getDevice());

                const API::QueueFamily* graphicsQueueFamily = renderer.getDevice().getQueueFamily(VK_QUEUE_GRAPHICS_BIT);
                if (!graphicsQueueFamily) {
                    LUG_LOG.error("Vulkan::Mesh::build: Can't find graphics queue");
                    return nullptr;
                }

                bufferBuilder.setQueueFamilyIndices({graphicsQueueFamily->getIdx()});
                bufferBuilder.setSize(targetPrimitiveSet.attributes[i].buffer.size);

                if (targetPrimitiveSet.attributes[i].type == lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Indice) {
                    bufferBuilder.setUsage(VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
                } else {
                    bufferBuilder.setUsage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
                }

                VkResult result{VK_SUCCESS};
                if (!bufferBuilder.build(buffer, &result)) {
                    LUG_LOG.error("Vulkan::Mesh::build: Can't create buffer: {}", result);
                    return nullptr;
                }

                primitiveSetData->buffers.push_back(std::move(buffer));
            }
        }

        primitiveSetData->pipelineIdPrimitivePart.positionVertexData = targetPrimitiveSet.position != nullptr;
        primitiveSetData->pipelineIdPrimitivePart.normalVertexData = targetPrimitiveSet.normal != nullptr;
        primitiveSetData->pipelineIdPrimitivePart.tangentVertexData = targetPrimitiveSet.tangent != nullptr;
        primitiveSetData->pipelineIdPrimitivePart.countTexCoord = targetPrimitiveSet.texCoords.size();
        primitiveSetData->pipelineIdPrimitivePart.primitiveMode = static_cast<uint32_t>(targetPrimitiveSet.mode);

        targetPrimitiveSet._data = static_cast<void*>(primitiveSetData);
        mesh->_primitiveSets.push_back(targetPrimitiveSet);
    }

    // Bind attributes buffers to mesh device memory
    {
        API::Builder::DeviceMemory deviceMemoryBuilder(renderer.getDevice());
        deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        for (auto& primitiveSet : mesh->_primitiveSets) {
            Render::Mesh::PrimitiveSetData* primitiveSetData = static_cast<Render::Mesh::PrimitiveSetData*>(primitiveSet._data);

            for (auto& buffer : primitiveSetData->buffers) {
                if (!deviceMemoryBuilder.addBuffer(buffer)) {
                    LUG_LOG.error("Vulkan::Mesh::build: Can't add buffer to device memory");
                    return nullptr;
                }
            }
        }

        VkResult result{VK_SUCCESS};
        if (!deviceMemoryBuilder.build(mesh->_deviceMemory, &result)) {
            LUG_LOG.error("Vulkan::Mesh::build: Can't create device memory: {}", result);
            return nullptr;
        }
    }

    return builder._renderer.getResourceManager()->add<::lug::Graphics::Render::Mesh>(std::move(resource));
}

} // Mesh
} // Builder
} // Vulkan
} // Graphics
} // lug
