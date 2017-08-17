#pragma once

#include <string>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Texture.hpp>
#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/API/Image.hpp>
#include <lug/Graphics/Vulkan/API/ImageView.hpp>
#include <lug/Graphics/Vulkan/API/Sampler.hpp>
#include <lug/Graphics/Vulkan/Builder/Texture.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

class LUG_GRAPHICS_API Texture final : public ::lug::Graphics::Render::Texture {
    friend Resource::SharedPtr<lug::Graphics::Render::Texture> Builder::Texture::build(const ::lug::Graphics::Builder::Texture&);

public:
    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

    ~Texture();

    const API::DeviceMemory& getDeviceMemory() const;
    const API::Image& getImage() const;
    const API::ImageView& getImageView() const;
    const API::Sampler& getSampler() const;

    void destroy();

private:
    Texture(const std::string& name);

private:
    API::DeviceMemory _deviceMemory{};
    API::Image _image{};
    API::ImageView _imageView{};
    API::Sampler _sampler{};
};

#include <lug/Graphics/Vulkan/Render/Texture.inl>

} // Render
} // Vulkan
} // Graphics
} // lug
