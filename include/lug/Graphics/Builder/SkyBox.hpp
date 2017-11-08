#pragma once

#include <array>
#include <cstdint>
#include <string>

#include <lug/Graphics/Builder/Texture.hpp>
#include <lug/Graphics/Resource.hpp>
#include <lug/Graphics/Render/SkyBox.hpp>
#include <lug/Graphics/Vulkan/Builder/SkyBox.hpp>

namespace lug {
namespace Graphics {

class Renderer;

namespace Builder {

class LUG_GRAPHICS_API SkyBox {
    friend Resource::SharedPtr<lug::Graphics::Render::SkyBox> lug::Graphics::Vulkan::Builder::SkyBox::build(const ::lug::Graphics::Builder::SkyBox&);

public:
    explicit SkyBox(Renderer& renderer);

    SkyBox(const SkyBox&) = delete;
    SkyBox(SkyBox&&) = delete;

    SkyBox& operator=(const SkyBox&) = delete;
    SkyBox& operator=(SkyBox&&) = delete;

    ~SkyBox() = default;

    /**
     * @brief      Sets the name.
     * @param[in]  name  The name of the mesh.
     */
    void setName(const std::string& name);

    void setMagFilter(Render::Texture::Filter magFilter);
    void setMinFilter(Render::Texture::Filter minFilter);
    void setMipMapFilter(Render::Texture::Filter mipMapFilter);
    void setWrapS(Render::Texture::WrappingMode wrapS);
    void setWrapT(Render::Texture::WrappingMode wrapT);
    void setWrapW(Render::Texture::WrappingMode wrapW);
    void setBackgroundFilename(const std::string& filename);
    void setEnvironnementFilename(const std::string& filename);

    Resource::SharedPtr<Render::SkyBox> build();

protected:
    Renderer& _renderer;

    std::string _name;

    Render::Texture::Filter _magFilter{Render::Texture::Filter::Nearest};
    Render::Texture::Filter _minFilter{Render::Texture::Filter::Nearest};
    Render::Texture::Filter _mipMapFilter{Render::Texture::Filter::Nearest};

    Render::Texture::WrappingMode _wrapS{Render::Texture::WrappingMode::ClampToEdge};
    Render::Texture::WrappingMode _wrapT{Render::Texture::WrappingMode::ClampToEdge};
    Render::Texture::WrappingMode _wrapW{Render::Texture::WrappingMode::ClampToEdge};

    std::string _backgroundFilename;
    std::string _environnementFilename;
};

#include <lug/Graphics/Builder/SkyBox.inl>

} // Builder
} // Graphics
} // lug
