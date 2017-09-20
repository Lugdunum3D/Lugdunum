#include <lug/Graphics/Vulkan/Builder/Mesh.hpp>

#include <lug/Graphics/Builder/Mesh.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Vulkan/Render/Material.hpp>
#include <lug/Graphics/Vulkan/Render/Mesh.hpp>
#include <lug/Graphics/Vulkan/Render/Pipeline.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Builder {
namespace Mesh {

Resource::SharedPtr<::lug::Graphics::Render::Mesh> build(const ::lug::Graphics::Builder::Mesh& builder) {
    // Constructor of Mesh is private, we can't use std::make_unique
    std::unique_ptr<Resource> resource{new Vulkan::Render::Mesh(builder._name)};
    Vulkan::Render::Mesh* mesh = static_cast<Vulkan::Render::Mesh*>(resource.get());

    Vulkan::Renderer& renderer = static_cast<Vulkan::Renderer&>(builder._renderer);

    const size_t primitiveSetsNb = builder._primitiveSets.size();
    mesh->_primitiveSets.resize(primitiveSetsNb);

    uint32_t i = 0;
    for (auto& builderPrimitiveSet : builder._primitiveSets) {
        Render::Mesh::PrimitiveSetData* primitiveSetData = new Render::Mesh::PrimitiveSetData();
        lug::Graphics::Render::Mesh::PrimitiveSet targetPrimitiveSet;

        targetPrimitiveSet.mode = builderPrimitiveSet.getMode();
        targetPrimitiveSet.material = builderPrimitiveSet.getMaterial();

        auto& builderAttributes = builderPrimitiveSet.getAttributes();

        const uint32_t attributesNb = static_cast<uint32_t>(builderAttributes.size());
        targetPrimitiveSet.attributes.resize(attributesNb);
        primitiveSetData->buffers.resize(attributesNb);

        for (uint32_t j = 0; j < attributesNb; ++j) {
            targetPrimitiveSet.attributes[j] = builderAttributes[j];

            // Pipeline::Handle::PrimitivePart support only 3 texture coordinates
            if (targetPrimitiveSet.attributes[j].type == lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::TexCoord &&
                targetPrimitiveSet.texCoords.size() == 3) {
                LUG_LOG.warn("Vulkan::Mesh::build: More than 3 texture coordinates, others will be ignored");
                continue;
            }

            switch (targetPrimitiveSet.attributes[j].type) {
                case lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Indice:
                    targetPrimitiveSet.indices = &targetPrimitiveSet.attributes[j];
                    break;
                case lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Position:
                    targetPrimitiveSet.position = &targetPrimitiveSet.attributes[j];
                    break;
                case lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Normal:
                    targetPrimitiveSet.normal = &targetPrimitiveSet.attributes[j];
                    break;
                case lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::TexCoord:
                    targetPrimitiveSet.texCoords.push_back(&targetPrimitiveSet.attributes[j]);
                    break;
                case lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Color:
                    targetPrimitiveSet.colors.push_back(&targetPrimitiveSet.attributes[j]);
                    break;
                case lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Tangent:
                    targetPrimitiveSet.tangent = &targetPrimitiveSet.attributes[j];
                    break;
            }

            {
                API::Buffer buffer;
                API::Builder::Buffer bufferBuilder(renderer.getDevice());

                const API::Queue* graphicsQueue = renderer.getDevice().getQueue("queue_graphics");
                if (!graphicsQueue) {
                    LUG_LOG.error("Vulkan::Mesh::build: Can't find graphics queue");
                    return nullptr;
                }

                bufferBuilder.setQueueFamilyIndices({graphicsQueue->getQueueFamily()->getIdx()});
                bufferBuilder.setSize(targetPrimitiveSet.attributes[j].buffer.size);

                if (targetPrimitiveSet.attributes[j].type == lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Indice) {
                    bufferBuilder.setUsage(VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
                } else {
                    bufferBuilder.setUsage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
                }

                VkResult result{VK_SUCCESS};
                if (!bufferBuilder.build(primitiveSetData->buffers[j], &result)) {
                    LUG_LOG.error("Vulkan::Mesh::build: Can't create buffer: {}", result);
                    return nullptr;
                }

                targetPrimitiveSet.attributes[j]._data = static_cast<void*>(&primitiveSetData->buffers[j]);
            }
        }

        primitiveSetData->pipelineIdPrimitivePart.positionVertexData = targetPrimitiveSet.position != nullptr;
        primitiveSetData->pipelineIdPrimitivePart.normalVertexData = targetPrimitiveSet.normal != nullptr;
        primitiveSetData->pipelineIdPrimitivePart.tangentVertexData = targetPrimitiveSet.tangent != nullptr;
        primitiveSetData->pipelineIdPrimitivePart.countTexCoord = targetPrimitiveSet.texCoords.size();
        primitiveSetData->pipelineIdPrimitivePart.countColor = targetPrimitiveSet.colors.size();
        primitiveSetData->pipelineIdPrimitivePart.primitiveMode = static_cast<uint32_t>(targetPrimitiveSet.mode);

        targetPrimitiveSet._data = static_cast<void*>(primitiveSetData);
        mesh->_primitiveSets[i] = std::move(targetPrimitiveSet);

        ++i;
    }

    // Bind attributes buffers to mesh device memory
    {
        API::Builder::DeviceMemory deviceMemoryBuilder(renderer.getDevice());
        // TODO(nokitoo): use memory flag VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

        for (auto& primitiveSet : mesh->_primitiveSets) {
            Render::Mesh::PrimitiveSetData* primitiveSetData = static_cast<Render::Mesh::PrimitiveSetData*>(primitiveSet._data);

            // Add buffers to memory
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

        // Update buffers data
        for (auto& primitiveSet : mesh->_primitiveSets) {
            Render::Mesh::PrimitiveSetData* primitiveSetData = static_cast<Render::Mesh::PrimitiveSetData*>(primitiveSet._data);

            uint32_t attributesNb = static_cast<uint32_t>(primitiveSet.attributes.size());

            for (i = 0; i < attributesNb; ++i) {
                primitiveSetData->buffers[i].updateData(
                    primitiveSet.attributes[i].buffer.data,
                    primitiveSet.attributes[i].buffer.size
                );
            }
        }
    }

    return builder._renderer.getResourceManager()->add<::lug::Graphics::Render::Mesh>(std::move(resource));
}

} // Mesh
} // Builder
} // Vulkan
} // Graphics
} // lug
