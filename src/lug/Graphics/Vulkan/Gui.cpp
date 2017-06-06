#include <imgui.h>

#include <lug\Graphics\Vulkan\Gui.hpp>
#include <lug\Graphics\Vulkan\API\Builder\CommandBuffer.hpp>
#include <lug\Graphics\Vulkan\API\Builder\CommandPool.hpp>
#include <lug\Graphics\Vulkan\API\Builder\DeviceMemory.hpp>
#include <lug\Graphics\Vulkan\API\Builder\ImageView.hpp>
#include <lug\Graphics\Vulkan\API\Builder\Image.hpp>
#include <lug\Graphics\Vulkan\API\Builder\Buffer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Gui::Gui(Renderer & renderer, Render::Window & window) : _renderer(renderer), _window(window) {
}

Gui::~Gui() {
    //vkDestroySampler(static_cast<VkDevice>(_renderer.getDevice()), _sampler, nullptr);
}

bool Gui::init(const std::vector<std::unique_ptr<API::ImageView>>& imageViews) {
    ImGuiIO& io = ImGui::GetIO();

    initKeyMapping();

    io.DisplaySize = ImVec2(_window.getWidth(), _window.getHeight());
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

    // Get present queue family and retrieve the first queue
    {
        _presentQueueFamily = _renderer.getDevice().getQueueFamily(VK_QUEUE_GRAPHICS_BIT);
        if (!_presentQueueFamily) {
            LUG_LOG.error("Gui::init: Can't find VK_QUEUE_GRAPHICS_BIT queue family");
            return false;
        }
        _presentQueue = &_presentQueueFamily->getQueues().front();
        if (!_presentQueue) {
            LUG_LOG.error("Gui::init: Can't find presentation queue");
            return false;
        }
    }

    // Create command pool of present queue
    {
        VkResult result{ VK_SUCCESS };
        API::Builder::CommandPool commandPoolBuilder(_renderer.getDevice(), *_presentQueueFamily);
        if (!commandPoolBuilder.build(_commandPool, &result)) {
            LUG_LOG.error("Gui::init: Can't create a command pool: {}", result);
            return false;
        }
    }
    API::Builder::CommandBuffer commandBufferBuilder(_renderer.getDevice(), _commandPool);

    // Command buffers
    {
        VkResult result{ VK_SUCCESS };
        if (!commandBufferBuilder.build(_commandBuffers, &result)) {
            LUG_LOG.error("Gui::init: Can't create the command buffer: {}", result);
            return false;
        }
    }

    _indexBuffers.resize(imageViews.size());
    _vertexBuffers.resize(imageViews.size());
    _vertexDeviceMemories.resize(imageViews.size());
    _indexDeviceMemories.resize(imageViews.size());
    _vertexCounts.resize(imageViews.size()); // NOTE: These might be unecessary as we might be able to retrieve their size 
    _indexCounts.resize(imageViews.size()); // NOTE: "  "

    return initFontsTexture() && initPipeline() && initFramebuffers(imageViews);
}

void Gui::initKeyMapping() {
    ImGuiIO& io = ImGui::GetIO();

    // Keyboard mapping for specific shortcuts used by ImGui
    io.KeyMap[ImGuiKey_Tab] = static_cast<int>(lug::Window::Keyboard::Key::Tab);
    io.KeyMap[ImGuiKey_LeftArrow] = static_cast<int>(lug::Window::Keyboard::Key::Left);
    io.KeyMap[ImGuiKey_RightArrow] = static_cast<int>(lug::Window::Keyboard::Key::Right);
    io.KeyMap[ImGuiKey_UpArrow] = static_cast<int>(lug::Window::Keyboard::Key::Up);
    io.KeyMap[ImGuiKey_DownArrow] = static_cast<int>(lug::Window::Keyboard::Key::Down);
    io.KeyMap[ImGuiKey_PageUp] = static_cast<int>(lug::Window::Keyboard::Key::PageUp);
    io.KeyMap[ImGuiKey_PageDown] = static_cast<int>(lug::Window::Keyboard::Key::PageDown);
    io.KeyMap[ImGuiKey_Home] = static_cast<int>(lug::Window::Keyboard::Key::Home);
    io.KeyMap[ImGuiKey_End] = static_cast<int>(lug::Window::Keyboard::Key::End);
    io.KeyMap[ImGuiKey_Delete] = static_cast<int>(lug::Window::Keyboard::Key::Delete);
    io.KeyMap[ImGuiKey_Backspace] = static_cast<int>(lug::Window::Keyboard::Key::BackSpace);
    io.KeyMap[ImGuiKey_Enter] = static_cast<int>(lug::Window::Keyboard::Key::Return);
    io.KeyMap[ImGuiKey_Escape] = static_cast<int>(lug::Window::Keyboard::Key::Escape);
    io.KeyMap[ImGuiKey_A] = static_cast<int>(lug::Window::Keyboard::Key::A);
    io.KeyMap[ImGuiKey_C] = static_cast<int>(lug::Window::Keyboard::Key::C);
    io.KeyMap[ImGuiKey_V] = static_cast<int>(lug::Window::Keyboard::Key::V);
    io.KeyMap[ImGuiKey_X] = static_cast<int>(lug::Window::Keyboard::Key::X);
    io.KeyMap[ImGuiKey_Y] = static_cast<int>(lug::Window::Keyboard::Key::Y);
    io.KeyMap[ImGuiKey_Z] = static_cast<int>(lug::Window::Keyboard::Key::Z);
}

bool Gui::initFontsTexture() {
    ImGuiIO& io = ImGui::GetIO();

    // Create font texture
    unsigned char* fontData;
    int texWidth, texHeight;
    io.Fonts->GetTexDataAsRGBA32(&fontData, &texWidth, &texHeight);
    size_t uploadSize = texWidth * texHeight * 4 * sizeof(char);

    API::Device &device = _renderer.getDevice();

    // Get transfer queue family and retrieve the first queue
    {
        _transferQueueFamily = device.getQueueFamily(VK_QUEUE_TRANSFER_BIT);
        if (!_transferQueueFamily) {
            LUG_LOG.error("Gui::initFontsTexture: Can't find VK_QUEUE_TRANSFER_BIT queue family");
            return false;
        }
        _transferQueue = &_transferQueueFamily->getQueues().front();
        if (!_transferQueue) {
            LUG_LOG.error("Gui::initFontsTexture: Can't find transfer queue");
            return false;
        }
    }

    // Create FontsTexture image
    {
        API::Builder::Image imageBuilder(device);

        imageBuilder.setUsage(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
        imageBuilder.setPreferedFormats({ VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT });
        imageBuilder.setFeatureFlags(VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
        imageBuilder.setTiling(VK_IMAGE_TILING_OPTIMAL);

        API::Builder::DeviceMemory deviceMemoryBuilder(device);
        deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        VkExtent3D extent{
            extent.width = texWidth,
            extent.height = texHeight,
            extent.depth = 1
        };

        imageBuilder.setExtent(extent);

        {
            VkResult result{ VK_SUCCESS };
            if (!imageBuilder.build(*_image, &result)) {
                LUG_LOG.error("Gui::initFontsTexture: Can't create depth buffer image: {}", result);
                return false;
            }

            if (!deviceMemoryBuilder.addImage(*_image)) {
                LUG_LOG.error("Gui::initFontsTexture: Can't add image to device memory");
                return false;
            }
        }
    }

    // Create FontsTexture image view
    {
        API::Builder::ImageView imageViewBuilder(device, *_image);

        imageViewBuilder.setFormat(_image->getFormat());
        imageViewBuilder.setAspectFlags(VK_IMAGE_ASPECT_COLOR_BIT);

        VkResult result{ VK_SUCCESS };
        if (!imageViewBuilder.build(*_imageView, &result)) {
            LUG_LOG.error("Gui::initFontsTexture: Can't create image view: {}", result);
            return false;
        }
    }

    // Create staging buffers for font data upload
    {
        // # # # NEW
        std::set<uint32_t> queueFamilyIndices = { (uint32_t)_transferQueueFamily->getIdx() };
        
        // Create staging buffer
        {
            API::Builder::Buffer bufferBuilder(device);
            bufferBuilder.setQueueFamilyIndices(queueFamilyIndices);
            bufferBuilder.setSize(uploadSize);
            bufferBuilder.setUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
            bufferBuilder.setExclusive(VK_SHARING_MODE_EXCLUSIVE);

            VkResult result{ VK_SUCCESS };
            if (!bufferBuilder.build(_stagingBuffer, &result)) {
                LUG_LOG.error("Gui::initFontsTexture: Can't create staging buffer: {}", result);
                return false;
            }
        }

        // # # # OLD
        auto& requirements = _stagingBuffer.getRequirements();
        uint32_t memoryTypeIndex = API::DeviceMemory::findMemoryType(device, requirements, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        auto fontsTextureDeviceMemory = API::DeviceMemory::allocate(device, requirements.size, memoryTypeIndex);
        if (!fontsTextureDeviceMemory) {
            LUG_LOG.error("GUI: Can't allocate device memory");
            return false;
        }

        _stagingBuffer.bindMemory(fontsTextureDeviceMemory.get());
        _stagingBuffer.updateData(fontData, (uint32_t)uploadSize);

        // Copy buffer data to font image
        {
            auto commandBuffer = transfertQueue->getCommandPool().createCommandBuffers();

            VkFence fence;
            VkFenceCreateInfo createInfo{
                createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                createInfo.pNext = nullptr,
                createInfo.flags = 0
            };
            VkResult result = vkCreateFence(static_cast<VkDevice>(_renderer.getDevice()), &createInfo, nullptr, &fence);
            if (result != VK_SUCCESS) {
                LUG_LOG.error("GUI: Can't create swapchain fence: {}", result);
                return false;
            }
            auto _fence = Vulkan::API::Fence(fence, &_renderer.getDevice());

            commandBuffer[0].begin();
            // Prepare for transfer
            _image->changeLayout(commandBuffer[0], 0, VK_ACCESS_TRANSFER_WRITE_BIT,
                VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

            // Copy
            VkBufferImageCopy bufferCopyRegion = {};
            bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            bufferCopyRegion.imageSubresource.layerCount = 1;
            bufferCopyRegion.imageExtent.width = texWidth;
            bufferCopyRegion.imageExtent.height = texHeight;
            bufferCopyRegion.imageExtent.depth = 1;

            vkCmdCopyBufferToImage(
                static_cast<VkCommandBuffer>(commandBuffer[0]),
                static_cast<VkBuffer>(*stagingBuffer),
                static_cast<VkImage>(*_image),
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1,
                &bufferCopyRegion
            );

            // Prepare for shader read
            _image->changeLayout(commandBuffer[0], VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

            commandBuffer[0].end();
            if (transfertQueue->submit(commandBuffer[0], {}, {}, {}, fence) == false) {
                LUG_LOG.error("GUI: Can't submit commandBuffer");
                return false;
            }

            // TODO : set a define for the fence timeout
            if (!_fence.wait()) {
                LUG_LOG.error("Gui: Can't vkWaitForFences");
                return false;
            }

            _fence.destroy();
            commandBuffer[0].destroy();
            stagingBuffer->destroy();
        }
    }

    return true;
}

bool Gui::initFramebuffers(const std::vector<std::unique_ptr<API::ImageView>>& /*imageViews*/) {
    return false;
}

bool Gui::initPipeline() {
    return false;
}

} // Vulkan
} // Graphics
} // lug