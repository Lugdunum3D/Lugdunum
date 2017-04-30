#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Resource.hpp>
#include <lug/Graphics/Vulkan/API/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/API/RenderPass.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class CommandBuffer;
class Device;

class LUG_GRAPHICS_API Pipeline final: public Resource {
public:
    struct Handle {
      struct PrimitivePart {
        union {
          struct {
            uint32_t positionVertexData : 1;
            uint32_t normalVertexData : 1;
            uint32_t tangentVertexData : 1;
            uint32_t countTexCoord : 2;
            uint32_t primitiveMode : 3;
          };

          uint32_t value;
        };

        explicit operator uint32_t() {
          return value;
        }
      };

      struct MaterialPart {
        union {
          struct {
            uint32_t countMaterialTextures : 3;
          };

          uint32_t value;
        };

        explicit operator uint32_t() {
          return value;
        }
      };

      struct PipelinePart {
        union {
          struct {
            uint32_t technique : 1;
          };

          uint32_t value;
        };

        explicit operator uint32_t() {
          return value;
        }
      };

      union {
        struct {
          uint32_t primitivePart : 8;
          uint32_t materialPart : 3;
          uint32_t pipelinePart : 1;
        };

        uint32_t value;
      };

      explicit operator uint32_t() {
        return value;
      }

      static Handle create(PrimitivePart primitivePart, MaterialPart materialPart, PipelinePart pipelinePart) {
        Handle handle;

        handle.primitivePart = static_cast<uint32_t>(primitivePart);
        handle.materialPart = static_cast<uint32_t>(materialPart);
        handle.pipelinePart = static_cast<uint32_t>(pipelinePart);

        return handle;
      };
    };
public:
    explicit Pipeline(VkPipeline pipeline = VK_NULL_HANDLE, const Device* device = nullptr,
        std::unique_ptr<PipelineLayout> layout = nullptr, std::unique_ptr<RenderPass> renderPass = nullptr);

    Pipeline(const Pipeline&) = delete;
    Pipeline(Pipeline&& pipeline);

    Pipeline& operator=(const Pipeline&) = delete;
    Pipeline& operator=(Pipeline&& pipeline);

    ~Pipeline();

    explicit operator VkPipeline() const {
        return _pipeline;
    }

    void bind(const CommandBuffer* commandBuffer);

    void destroy();

    RenderPass* getRenderPass() const;
    PipelineLayout* getLayout() const;

    static std::unique_ptr<Pipeline> createGraphicsPipeline(const Device* device, const std::string& vertexShaderFile, const std::string& fragmentShaderFile, VkFormat colorFormat);

private:
    VkPipeline _pipeline{VK_NULL_HANDLE};
    const Device* _device{nullptr};
    std::unique_ptr<PipelineLayout> _layout;
    std::unique_ptr<RenderPass> _renderPass;
};

} // API
} // Vulkan
} // Graphics
} // lug
