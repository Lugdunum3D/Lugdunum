#pragma once

#include <list>

#include <lug/Graphics/Vulkan/API/RenderPass.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;

namespace Builder {

class RenderPass {
public:
    using AttachmentIndex = const VkAttachmentDescription*;
    using SubpassIndex = const VkSubpassDescription*;

    struct AttachmentReference {
        AttachmentIndex attachment{nullptr};
        VkImageLayout layout{VK_IMAGE_LAYOUT_UNDEFINED};
    };

    struct SubpassDescription {
        VkPipelineBindPoint pipelineBindPoint{VK_PIPELINE_BIND_POINT_GRAPHICS};
        std::vector<AttachmentReference> inputAttachments{};
        std::vector<AttachmentReference> colorAttachments{};
        std::vector<AttachmentReference> resolveAttachments{};
        AttachmentReference depthStencilAttachment{};
        std::vector<AttachmentIndex> preserveAttachments{};
    };

    struct SubpassDependency {
        SubpassIndex srcSubpass{nullptr};
        SubpassIndex dstSubpass{nullptr};
        VkPipelineStageFlags srcStageMask{0};
        VkPipelineStageFlags dstStageMask{0};
        VkAccessFlags srcAccessMask{0};
        VkAccessFlags dstAccessMask{0};
        VkDependencyFlags dependencyFlags{0};
    };

public:
    RenderPass(const API::Device& device);

    RenderPass(const RenderPass&&) = delete;
    RenderPass(RenderPass&&) = delete;

    RenderPass& operator=(const RenderPass&&) = delete;
    RenderPass& operator=(RenderPass&&) = delete;

    ~RenderPass() = default;

    // Setters
    AttachmentIndex addAttachment(const VkAttachmentDescription& attachmentDescription);
    SubpassIndex addSubpass(const SubpassDescription& subpassDescription);
    void addSubpassDependency(const SubpassDependency& subpassDependency);

    // Build methods
    bool build(API::RenderPass& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::RenderPass> build(VkResult* returnResult = nullptr);

private:
    struct SubpassResources {
        std::vector<VkAttachmentReference> inputAttachments;
        std::vector<VkAttachmentReference> colorAttachments;
        std::vector<VkAttachmentReference> resolveAttachments;
        VkAttachmentReference depthStencilAttachment;
        std::vector<uint32_t> preserveAttachments;
    };

private:
    const API::Device& _device;

    std::list<VkAttachmentDescription> _attachments;
    std::list<VkSubpassDescription> _subpasses;
    std::list<SubpassResources> _subpassesResources;
    std::list<VkSubpassDependency> _dependencies;
};

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
