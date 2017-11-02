#include <lug/Graphics/Vulkan/Builder/Texture.hpp>

#include <lug/Graphics/Builder/Texture.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Fence.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Sampler.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Vulkan/Render/Texture.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Builder {
namespace Texture {

Resource::SharedPtr<::lug::Graphics::Render::Texture> build(const ::lug::Graphics::Builder::Texture& builder) {
    // Constructor of Texture is private, we can't use std::make_unique
    std::unique_ptr<Resource> resource{new Vulkan::Render::Texture(builder._name)};
    Vulkan::Render::Texture* texture = static_cast<Vulkan::Render::Texture*>(resource.get());

    if (!builder._layers.size()) {
        LUG_LOG.error("Vulkan::Texture::build: Not layers added");
        return nullptr;
    }

    Vulkan::Renderer& renderer = static_cast<Vulkan::Renderer&>(builder._renderer);
    API::Device &device = renderer.getDevice();

    // Get transfer queue family and retrieve the first queue
    const API::Queue* transferQueue = nullptr;
    {
        transferQueue = device.getQueue("queue_transfer");
        if (!transferQueue) {
            LUG_LOG.error("Vulkan::Texture::build: Can't find transfer queue");
            return nullptr;
        }
    }

    // Create command pool of transfer queue
    API::CommandPool transferQueueCommandPool;
    {
        VkResult result{VK_SUCCESS};
        API::Builder::CommandPool commandPoolBuilder(device, *transferQueue->getQueueFamily());
        if (!commandPoolBuilder.build(transferQueueCommandPool, &result)) {
            LUG_LOG.error("Vulkan::Texture::build: Can't create a command pool: {}", result);
            return nullptr;
        }
    }

    // Create the API::Image
    {
        API::Builder::Image imageBuilder(device);

        const VkFormat format = [](Render::Texture::Format format) {
            switch(format) {
                case Render::Texture::Format::R8G8B8A8_UNORM:
                    return VK_FORMAT_R8G8B8A8_UNORM;

                case Render::Texture::Format::R16G16_SFLOAT:
                    return VK_FORMAT_R16G16_SFLOAT;

                case Render::Texture::Format::R16G16B16_SFLOAT:
                    return VK_FORMAT_R16G16B16_SFLOAT;

                default:
                    return VK_FORMAT_UNDEFINED;
            };
        }(builder._format);

        // TODO: Take the usage from the builder (TRANSFER_DST only if we want to copy image to it, etc)
        imageBuilder.setUsage(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
        imageBuilder.setPreferedFormats({format});
        imageBuilder.setFeatureFlags(VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
        imageBuilder.setQueueFamilyIndices({ transferQueue->getQueueFamily()->getIdx() });
        imageBuilder.setTiling(VK_IMAGE_TILING_OPTIMAL);
        imageBuilder.setMipLevels(builder._mipLevels);
        imageBuilder.setArrayLayers(static_cast<uint32_t>(builder._layers.size()));

        API::Builder::DeviceMemory deviceMemoryBuilder(device);
        deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        VkExtent3D extent{
            /* extent.width */ static_cast<uint32_t>(builder._width),
            /* extent.height */ static_cast<uint32_t>(builder._height),
            /* extent.depth */ 1
        };

        imageBuilder.setExtent(extent);

        if (builder._type == ::lug::Graphics::Builder::Texture::Type::CubeMap) {
            imageBuilder.setCreateFlags(VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT);
        }

        {
            VkResult result{VK_SUCCESS};
            if (!imageBuilder.build(texture->_image, &result)) {
                LUG_LOG.error("Vulkan::Texture::build: Can't create the image: {}", result);
                return nullptr;
            }

            if (!deviceMemoryBuilder.addImage(texture->_image)) {
                LUG_LOG.error("Vulkan::Texture::build: Can't add image to device memory");
                return nullptr;
            }

            result = VK_SUCCESS;
            if (!deviceMemoryBuilder.build(texture->_deviceMemory, &result)) {
                LUG_LOG.error("Vulkan::Texture::build: Can't create buffer device memory: {}", result);
                return nullptr;
            }
        }
    }

    // Create image view
    {
        API::Builder::ImageView imageViewBuilder(device, texture->_image);

        imageViewBuilder.setFormat(texture->_image.getFormat());
        imageViewBuilder.setAspectFlags(VK_IMAGE_ASPECT_COLOR_BIT);
        imageViewBuilder.setLayerCount(static_cast<uint32_t>(builder._layers.size()));
        imageViewBuilder.setLevelCount(builder._mipLevels);

        if (builder._type == ::lug::Graphics::Builder::Texture::Type::CubeMap) {
            imageViewBuilder.setViewType(VK_IMAGE_VIEW_TYPE_CUBE);
        }

        VkResult result{VK_SUCCESS};
        if (!imageViewBuilder.build(texture->_imageView, &result)) {
            LUG_LOG.error("Vulkan::Texture::build: Can't create image view: {}", result);
            return nullptr;
        }
    }

    uint32_t nbLayersWithData = 0;
    for (const auto& layer : builder._layers) {
        if (layer.data) {
            ++nbLayersWithData;
        }
    }

    // Create staging buffers for image upload
    // The number of layers is not neccessarily equals to builder._layers.size() (5 layers and 2 filenames)
    if (nbLayersWithData)
    {
        // TODO: Replace 4 with the value depending on the format
        const VkDeviceSize layerSize = builder._width * builder._height * 4;
        const VkDeviceSize bufferSize = layerSize * nbLayersWithData;

        API::Buffer stagingBuffer;
        API::DeviceMemory stagingBufferMemory;
        std::set<uint32_t> queueFamilyIndices = { transferQueue->getQueueFamily()->getIdx() };

        // Create staging buffer
        {
            API::Builder::Buffer bufferBuilder(device);
            bufferBuilder.setQueueFamilyIndices(queueFamilyIndices);
            bufferBuilder.setSize(bufferSize);
            bufferBuilder.setUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
            bufferBuilder.setExclusive(VK_SHARING_MODE_EXCLUSIVE);

            VkResult result{VK_SUCCESS};
            if (!bufferBuilder.build(stagingBuffer, &result)) {
                LUG_LOG.error("Vulkan::Texture::build: Can't create staging buffer: {}", result);
                return nullptr;
            }
        }

        // Create staging buffer memory
        {
            API::Builder::DeviceMemory deviceMemoryBuilder(device);
            deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            deviceMemoryBuilder.addBuffer(stagingBuffer);

            VkResult result{VK_SUCCESS};
            if (!deviceMemoryBuilder.build(stagingBufferMemory, &result)) {
                LUG_LOG.error("Vulkan::Texture::build: Can't create staging buffer device memory: {}", result);
                return nullptr;
            }
        }

        // Update buffer data
        {
            VkDeviceSize pixelsOffset{0};
            for (const auto& layer : builder._layers) {
                if (!layer.data) {
                    continue;
                }

                stagingBuffer.updateData(layer.data, layerSize, pixelsOffset);
                pixelsOffset += layerSize;
            }
        }


        // Copy buffer data to font image
        {
            VkResult result{VK_SUCCESS};
            API::CommandBuffer commandBuffer;

            API::Builder::CommandBuffer commandBufferBuilder(device, transferQueueCommandPool);
            commandBufferBuilder.setLevel(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

            if (!commandBufferBuilder.build(commandBuffer, &result)) {
                LUG_LOG.error("Gui::init: Can't create the command buffer: {}", result);
                return nullptr;
            }

            // Create fence
            API::Fence fence;
            {
                API::Builder::Fence fenceBuilder(device);

                if (!fenceBuilder.build(fence, &result)) {
                    LUG_LOG.error("Vulkan::Texture::build: Can't create swapchain fence: {}", result);
                    return nullptr;
                }
            }

            commandBuffer.begin();

            // Prepare for transfer
            {
                API::CommandBuffer::CmdPipelineBarrier pipelineBarrier;
                pipelineBarrier.imageMemoryBarriers.resize(1);
                pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = 0;
                pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].image = &texture->_image;
                pipelineBarrier.imageMemoryBarriers[0].subresourceRange.layerCount = static_cast<uint32_t>(builder._layers.size());

                commandBuffer.pipelineBarrier(pipelineBarrier);
            }

            std::vector<VkBufferImageCopy> bufferCopyRegions(nbLayersWithData);
            VkDeviceSize pixelsOffset{0};
            uint32_t i = 0;
            for (uint32_t layerNb = 0; layerNb < builder._layers.size(); ++layerNb) {
                if (!builder._layers[layerNb].data) {
                    continue;
                }

                // Copy
                bufferCopyRegions[i++] = {
                    /* bufferCopyRegion.bufferOffset */ pixelsOffset,
                    /* bufferCopyRegion.bufferRowLength */ 0,
                    /* bufferCopyRegion.bufferImageHeight */ 0,
                    {
                        /* bufferCopyRegion.imageSubresource.aspectMask */ VK_IMAGE_ASPECT_COLOR_BIT,
                        /* bufferCopyRegion.imageSubresource.mipLevel */ 0,
                        /* bufferCopyRegion.imageSubresource.baseArrayLayer */ layerNb,
                        /* bufferCopyRegion.imageSubresource.layerCount */ 1
                    },
                    {
                        /* bufferCopyRegion.imageOffset.x */ 0,
                        /* bufferCopyRegion.imageOffset.y */ 0,
                        /* bufferCopyRegion.imageOffset.z */ 0,
                    },
                    {
                        /* bufferCopyRegion.imageExtent.width */ static_cast<uint32_t>(builder._width),
                        /* bufferCopyRegion.imageExtent.height */ static_cast<uint32_t>(builder._height),
                        /* bufferCopyRegion.imageExtent.depth */ 1
                    }
                };

                pixelsOffset += layerSize;
            }


            // TODO write this function commandBuffer.copyBufferToImage();

            vkCmdCopyBufferToImage(
                static_cast<VkCommandBuffer>(commandBuffer),
                static_cast<VkBuffer>(stagingBuffer),
                static_cast<VkImage>(texture->_image),
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                static_cast<uint32_t>(bufferCopyRegions.size()),
                bufferCopyRegions.data()
            );

            // Prepare for shader read
            {
                API::CommandBuffer::CmdPipelineBarrier pipelineBarrier;
                pipelineBarrier.imageMemoryBarriers.resize(1);
                pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].image = &texture->_image;
                pipelineBarrier.imageMemoryBarriers[0].subresourceRange.layerCount = static_cast<uint32_t>(builder._layers.size());

                commandBuffer.pipelineBarrier(pipelineBarrier);
            }

            if (commandBuffer.end() == false) {
                LUG_LOG.error("Vulkan::Texture::build: Failed to end commandBuffer");
                return nullptr;
            }

            if (transferQueue->submit(commandBuffer, {}, {}, {}, static_cast<VkFence>(fence)) == false) {
                LUG_LOG.error("Vulkan::Texture::build: Can't submit commandBuffer");
                return nullptr;
            }

            // TODO(saveman71): set a define for the fence timeout
            if (!fence.wait()) {
                LUG_LOG.error("Vulkan::Texture::build: Can't vkWaitForFences");
                return nullptr;
            }

            // Properly destroy everything in the right order
            fence.destroy();
            stagingBufferMemory.destroy();
            stagingBuffer.destroy();
            commandBuffer.destroy();
        }
    }

    // Create Sampler
    {
        API::Builder::Sampler samplerBuilder(device);

        const auto& wrappingModeToVulkan = [](Render::Texture::WrappingMode wrappingMode){
            switch(wrappingMode) {
                case Render::Texture::WrappingMode::ClampToEdge:
                    return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
                case Render::Texture::WrappingMode::MirroredRepeat:
                    return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
                case Render::Texture::WrappingMode::Repeat:
                    return VK_SAMPLER_ADDRESS_MODE_REPEAT;
            }

            return VkSamplerAddressMode{};
        };

        samplerBuilder.setAddressModeU(wrappingModeToVulkan(builder._wrapS));
        samplerBuilder.setAddressModeV(wrappingModeToVulkan(builder._wrapT));
        samplerBuilder.setAddressModeW(wrappingModeToVulkan(builder._wrapW));

        const auto& filterToVulkan = [](Render::Texture::Filter filter){
            switch(filter) {
                case Render::Texture::Filter::Nearest:
                    return VK_FILTER_NEAREST;
                case Render::Texture::Filter::Linear:
                    return VK_FILTER_LINEAR;
            }

            return VkFilter{};
        };

        samplerBuilder.setMinFilter(filterToVulkan(builder._minFilter));
        samplerBuilder.setMagFilter(filterToVulkan(builder._magFilter));

        samplerBuilder.setMipmapMode([](Render::Texture::Filter filter){
            switch(filter) {
                case Render::Texture::Filter::Nearest:
                    return VK_SAMPLER_MIPMAP_MODE_NEAREST;
                case Render::Texture::Filter::Linear:
                    return VK_SAMPLER_MIPMAP_MODE_LINEAR;
            }

            return VkSamplerMipmapMode{};
        }(builder._mipMapFilter));

        samplerBuilder.setMaxLod(static_cast<float>(builder._mipLevels));

        VkResult result{VK_SUCCESS};
        if (!samplerBuilder.build(texture->_sampler, &result)) {
            LUG_LOG.error("Gui::initFontsTexture: Can't create image view: {}", result);
            return nullptr;
        }
    }

    return builder._renderer.getResourceManager()->add<::lug::Graphics::Render::Texture>(std::move(resource));
}

} // Texture
} // Builder
} // Vulkan
} // Graphics
} // lug
