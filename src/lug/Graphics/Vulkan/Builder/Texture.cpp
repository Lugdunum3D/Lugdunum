#include <lug/Graphics/Vulkan/Builder/Texture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(builder._filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    if (!pixels) {
        LUG_LOG.error("Vulkan::Texture::build: Failed to load the image");
        return nullptr;
    }

    // Create the API::Image
    {
        API::Builder::Image imageBuilder(device);

        imageBuilder.setUsage(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
        imageBuilder.setPreferedFormats({ VK_FORMAT_R8G8B8A8_UNORM });
        imageBuilder.setFeatureFlags(VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
        imageBuilder.setQueueFamilyIndices({ transferQueue->getQueueFamily()->getIdx() });
        imageBuilder.setTiling(VK_IMAGE_TILING_OPTIMAL);

        API::Builder::DeviceMemory deviceMemoryBuilder(device);
        deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        VkExtent3D extent{
            /* extent.width */ static_cast<uint32_t>(texWidth),
            /* extent.height */ static_cast<uint32_t>(texHeight),
            /* extent.depth */ 1
        };

        imageBuilder.setExtent(extent);

        {
            VkResult result{VK_SUCCESS};
            if (!imageBuilder.build(texture->_image, &result)) {
                LUG_LOG.error("Vulkan::Texture::build: Can't create the image: {}", result);
                stbi_image_free(pixels);
                return nullptr;
            }

            if (!deviceMemoryBuilder.addImage(texture->_image)) {
                LUG_LOG.error("Vulkan::Texture::build: Can't add image to device memory");
                stbi_image_free(pixels);
                return nullptr;
            }

            result = VK_SUCCESS;
            if (!deviceMemoryBuilder.build(texture->_deviceMemory, &result)) {
                LUG_LOG.error("Vulkan::Texture::build: Can't create buffer device memory: {}", result);
                stbi_image_free(pixels);
                return nullptr;
            }
        }
    }

    // Create image view
    {
        API::Builder::ImageView imageViewBuilder(device, texture->_image);

        imageViewBuilder.setFormat(texture->_image.getFormat());
        imageViewBuilder.setAspectFlags(VK_IMAGE_ASPECT_COLOR_BIT);

        VkResult result{VK_SUCCESS};
        if (!imageViewBuilder.build(texture->_imageView, &result)) {
            LUG_LOG.error("Vulkan::Texture::build: Can't create image view: {}", result);
            stbi_image_free(pixels);
            return nullptr;
        }
    }

    // Create staging buffers for image upload
    {
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        API::Buffer stagingBuffer;
        API::DeviceMemory stagingBufferMemory;
        std::set<uint32_t> queueFamilyIndices = { transferQueue->getQueueFamily()->getIdx() };

        // Create staging buffer
        {
            API::Builder::Buffer bufferBuilder(device);
            bufferBuilder.setQueueFamilyIndices(queueFamilyIndices);
            bufferBuilder.setSize(imageSize);
            bufferBuilder.setUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
            bufferBuilder.setExclusive(VK_SHARING_MODE_EXCLUSIVE);

            VkResult result{VK_SUCCESS};
            if (!bufferBuilder.build(stagingBuffer, &result)) {
                LUG_LOG.error("Vulkan::Texture::build: Can't create staging buffer: {}", result);
                stbi_image_free(pixels);
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
                stbi_image_free(pixels);
                return nullptr;
            }
        }

        stagingBuffer.updateData(pixels, static_cast<uint32_t>(imageSize));

        // Copy buffer data to font image
        {
            VkResult result{VK_SUCCESS};
            API::CommandBuffer commandBuffer;

            API::Builder::CommandBuffer commandBufferBuilder(device, transferQueueCommandPool);
            commandBufferBuilder.setLevel(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

            if (!commandBufferBuilder.build(commandBuffer, &result)) {
                LUG_LOG.error("Gui::init: Can't create the command buffer: {}", result);
                stbi_image_free(pixels);
                return nullptr;
            }

            // Create fence
            API::Fence fence;
            {
                API::Builder::Fence fenceBuilder(device);

                if (!fenceBuilder.build(fence, &result)) {
                    LUG_LOG.error("Vulkan::Texture::build: Can't create swapchain fence: {}", result);
                    stbi_image_free(pixels);
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

                commandBuffer.pipelineBarrier(pipelineBarrier);
            }

            // Copy
            VkBufferImageCopy bufferCopyRegion{
                /* bufferCopyRegion.bufferOffset */ 0,
                /* bufferCopyRegion.bufferRowLength */ 0,
                /* bufferCopyRegion.bufferImageHeight */ 0,
                {
                    /* bufferCopyRegion.imageSubresource.aspectMask */ VK_IMAGE_ASPECT_COLOR_BIT,
                    /* bufferCopyRegion.imageSubresource.mipLevel */ 0,
                    /* bufferCopyRegion.imageSubresource.baseArrayLayer */ 0,
                    /* bufferCopyRegion.imageSubresource.layerCount */ 1
                },
                {
                    /* bufferCopyRegion.imageOffset.x */ 0,
                    /* bufferCopyRegion.imageOffset.y */ 0,
                    /* bufferCopyRegion.imageOffset.z */ 0,
                },
                {
                    /* bufferCopyRegion.imageExtent.width */ static_cast<uint32_t>(texWidth),
                    /* bufferCopyRegion.imageExtent.height */ static_cast<uint32_t>(texHeight),
                    /* bufferCopyRegion.imageExtent.depth */ 1
                }
            };

            // TODO write this function commandBuffer.copyBufferToImage();

            vkCmdCopyBufferToImage(
                static_cast<VkCommandBuffer>(commandBuffer),
                static_cast<VkBuffer>(stagingBuffer),
                static_cast<VkImage>(texture->_image),
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1,
                &bufferCopyRegion
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

                commandBuffer.pipelineBarrier(pipelineBarrier);
            }

            if (commandBuffer.end() == false) {
                LUG_LOG.error("Vulkan::Texture::build: Failed to end commandBuffer");
                stbi_image_free(pixels);
                return nullptr;
            }

            if (transferQueue->submit(commandBuffer, {}, {}, {}, static_cast<VkFence>(fence)) == false) {
                LUG_LOG.error("Vulkan::Texture::build: Can't submit commandBuffer");
                stbi_image_free(pixels);
                return nullptr;
            }

            // TODO(saveman71): set a define for the fence timeout
            if (!fence.wait()) {
                LUG_LOG.error("Vulkan::Texture::build: Can't vkWaitForFences");
                stbi_image_free(pixels);
                return nullptr;
            }

            // Properly destroy everything in the right order
            fence.destroy();
            stagingBufferMemory.destroy();
            stagingBuffer.destroy();
            commandBuffer.destroy();

            // Free the data
            stbi_image_free(pixels);
        }
    }

    // Create Sampler
    {
        API::Builder::Sampler samplerBuilder(device);

        // TODO: Use wrapS, wrapT
        // TODO: Use minFilter, magFilter, mipMapFilter
        samplerBuilder.setAddressModeU(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        samplerBuilder.setAddressModeV(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        samplerBuilder.setAddressModeW(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        samplerBuilder.setBorderColor(VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE);

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
