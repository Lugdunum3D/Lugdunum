#include <lug/Graphics/Vulkan/Builder/Texture.hpp>

#if defined(LUG_SYSTEM_WINDOWS)
    #pragma warning(push)
    #pragma warning(disable : 4244)
    #pragma warning(disable : 4456)

#endif
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#if defined(LUG_SYSTEM_WINDOWS)
    #pragma warning(pop)
#endif

#if defined(LUG_SYSTEM_ANDROID)
    #include <android/asset_manager.h>

    #include <lug/Window/Android/WindowImplAndroid.hpp>
    #include <lug/Window/Window.hpp>
#endif

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

static void freePixels(std::vector<stbi_uc*> layersPixels) {
    for (auto& pixels: layersPixels) {
        stbi_image_free(pixels);
    }
}

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

    int texWidth{0};
    int texHeight{0};
    int texChannels{0};
    std::vector<stbi_uc*> layersPixels;
    for (auto& layer: builder._layers) {
        if (layer.filename.size() == 0 && layer.data) {
            layersPixels.push_back(layer.data);
            texWidth = layer.width;
            texHeight = layer.height;
            continue;
        }

#if defined(LUG_SYSTEM_ANDROID)
        // Load shader from compressed asset
        AAsset* asset = AAssetManager_open((lug::Window::priv::WindowImpl::activity)->assetManager, layer.filename.c_str(), AASSET_MODE_STREAMING);

        if (!asset) {
            LUG_LOG.error("Vulkan::Texture::build: Can't open Android asset \"{}\"", layer.filename);
            freePixels(layersPixels);
            return nullptr;
        }

        uint32_t size = AAsset_getLength(asset);

        if (size <= 0) {
            LUG_LOG.error("Vulkan::Texture::build: Android asset \"{}\" is empty", layer.filename);
            freePixels(layersPixels);
            return nullptr;
        }

        unsigned char* data = new unsigned char[size];

        AAsset_read(asset, reinterpret_cast<char*>(data), size);
        AAsset_close(asset);

        stbi_uc* pixels = stbi_load_from_memory(data, size, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

        delete[] data;
#else
        stbi_uc* pixels = stbi_load(layer.filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
#endif

        if (!pixels) {
            LUG_LOG.error("Vulkan::Texture::build: Failed to load the image \"{}\"", layer.filename);
            freePixels(layersPixels);
            return nullptr;
        }

        layersPixels.push_back(pixels);
    }

    // Create the API::Image
    {
        API::Builder::Image imageBuilder(device);

        imageBuilder.setUsage(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
        imageBuilder.setPreferedFormats({ VK_FORMAT_R8G8B8A8_UNORM });
        imageBuilder.setFeatureFlags(VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
        imageBuilder.setQueueFamilyIndices({ transferQueue->getQueueFamily()->getIdx() });
        imageBuilder.setTiling(VK_IMAGE_TILING_OPTIMAL);
        imageBuilder.setArrayLayers(static_cast<uint32_t>(layersPixels.size()));

        API::Builder::DeviceMemory deviceMemoryBuilder(device);
        deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        VkExtent3D extent{
            /* extent.width */ static_cast<uint32_t>(texWidth),
            /* extent.height */ static_cast<uint32_t>(texHeight),
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
                freePixels(layersPixels);
                return nullptr;
            }

            if (!deviceMemoryBuilder.addImage(texture->_image)) {
                LUG_LOG.error("Vulkan::Texture::build: Can't add image to device memory");
                freePixels(layersPixels);
                return nullptr;
            }

            result = VK_SUCCESS;
            if (!deviceMemoryBuilder.build(texture->_deviceMemory, &result)) {
                LUG_LOG.error("Vulkan::Texture::build: Can't create buffer device memory: {}", result);
                freePixels(layersPixels);
                return nullptr;
            }
        }
    }

    // Create image view
    {
        API::Builder::ImageView imageViewBuilder(device, texture->_image);

        imageViewBuilder.setFormat(texture->_image.getFormat());
        imageViewBuilder.setAspectFlags(VK_IMAGE_ASPECT_COLOR_BIT);
        imageViewBuilder.setLayerCount(static_cast<uint32_t>(layersPixels.size()));

        if (builder._type == ::lug::Graphics::Builder::Texture::Type::CubeMap) {
            imageViewBuilder.setViewType(VK_IMAGE_VIEW_TYPE_CUBE);
        }

        VkResult result{VK_SUCCESS};
        if (!imageViewBuilder.build(texture->_imageView, &result)) {
            LUG_LOG.error("Vulkan::Texture::build: Can't create image view: {}", result);
            freePixels(layersPixels);
            return nullptr;
        }
    }

    // Create staging buffers for image upload
    {
        VkDeviceSize imageSize = texWidth * texHeight * 4 * layersPixels.size();

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
                freePixels(layersPixels);
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
                freePixels(layersPixels);
                return nullptr;
            }
        }

        // Update buffer data
        VkDeviceSize layerSize{imageSize / layersPixels.size()};
        {
            VkDeviceSize pixelsOffset{0};
            for (stbi_uc* pixels: layersPixels) {
                stagingBuffer.updateData(pixels, layerSize, pixelsOffset);
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
                freePixels(layersPixels);
                return nullptr;
            }

            // Create fence
            API::Fence fence;
            {
                API::Builder::Fence fenceBuilder(device);

                if (!fenceBuilder.build(fence, &result)) {
                    LUG_LOG.error("Vulkan::Texture::build: Can't create swapchain fence: {}", result);
                    freePixels(layersPixels);
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
                pipelineBarrier.imageMemoryBarriers[0].subresourceRange.layerCount = static_cast<uint32_t>(layersPixels.size());

                commandBuffer.pipelineBarrier(pipelineBarrier);
            }

            std::vector<VkBufferImageCopy> bufferCopyRegions(layersPixels.size());
            VkDeviceSize pixelsOffset{0};
            for (uint32_t i = 0; i < layersPixels.size(); ++i) {
                // Copy
                VkBufferImageCopy bufferCopyRegion{
                    /* bufferCopyRegion.bufferOffset */ pixelsOffset,
                    /* bufferCopyRegion.bufferRowLength */ 0,
                    /* bufferCopyRegion.bufferImageHeight */ 0,
                    {
                        /* bufferCopyRegion.imageSubresource.aspectMask */ VK_IMAGE_ASPECT_COLOR_BIT,
                        /* bufferCopyRegion.imageSubresource.mipLevel */ 0,
                        /* bufferCopyRegion.imageSubresource.baseArrayLayer */ i,
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

                bufferCopyRegions[i] = bufferCopyRegion;
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

                commandBuffer.pipelineBarrier(pipelineBarrier);
            }

            if (commandBuffer.end() == false) {
                LUG_LOG.error("Vulkan::Texture::build: Failed to end commandBuffer");
                freePixels(layersPixels);
                return nullptr;
            }

            if (transferQueue->submit(commandBuffer, {}, {}, {}, static_cast<VkFence>(fence)) == false) {
                LUG_LOG.error("Vulkan::Texture::build: Can't submit commandBuffer");
                freePixels(layersPixels);
                return nullptr;
            }

            // TODO(saveman71): set a define for the fence timeout
            if (!fence.wait()) {
                LUG_LOG.error("Vulkan::Texture::build: Can't vkWaitForFences");
                freePixels(layersPixels);
                return nullptr;
            }

            // Properly destroy everything in the right order
            fence.destroy();
            stagingBufferMemory.destroy();
            stagingBuffer.destroy();
            commandBuffer.destroy();

            // Free the data
            freePixels(layersPixels);
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
