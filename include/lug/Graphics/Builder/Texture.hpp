#pragma once

#include <string>

#include <lug/Graphics/Resource.hpp>
#include <lug/Graphics/Render/Texture.hpp>
#include <lug/Graphics/Vulkan/Builder/Texture.hpp>

namespace lug {
namespace Graphics {

class Renderer;

namespace Builder {

class LUG_GRAPHICS_API Texture {
    friend Resource::SharedPtr<lug::Graphics::Render::Texture> lug::Graphics::Vulkan::Builder::Texture::build(const ::lug::Graphics::Builder::Texture&);

public:
    explicit Texture(Renderer& renderer);

    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

    ~Texture() = default;

    /**
     * @brief      Sets the name.
     * @param[in]  name  The name of the mesh.
     */
    void setName(const std::string& name);

    void setFilename(const std::string& filename);

    void setMagFilter(Render::Texture::Filter magFilter);
    void setMinFilter(Render::Texture::Filter minFilter);
    void setMipMapFilter(Render::Texture::Filter mipMapFilter);
    void setWrapS(Render::Texture::WrappingMode wrapS);
    void setWrapT(Render::Texture::WrappingMode wrapT);

    Resource::SharedPtr<Render::Texture> build();

protected:
    Renderer& _renderer;

    std::string _name;

    std::string _filename;
    Render::Texture::Filter _magFilter{Render::Texture::Filter::Nearest};
    Render::Texture::Filter _minFilter{Render::Texture::Filter::Nearest};
    Render::Texture::Filter _mipMapFilter{Render::Texture::Filter::Nearest};

    Render::Texture::WrappingMode _wrapS{Render::Texture::WrappingMode::ClampToEdge};
    Render::Texture::WrappingMode _wrapT{Render::Texture::WrappingMode::ClampToEdge};
};

#include <lug/Graphics/Builder/Texture.inl>

} // Builder
} // Graphics
} // lug
