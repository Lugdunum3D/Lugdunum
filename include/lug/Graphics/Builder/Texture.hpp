#pragma once

#include <string>
#include <vector>

#include <lug/Graphics/Resource.hpp>
#include <lug/Graphics/Render/Texture.hpp>
#include <lug/Graphics/Vulkan/Builder/Texture.hpp>

namespace lug {
namespace Graphics {

class Renderer;

namespace Builder {

class LUG_GRAPHICS_API Texture {
    friend Resource::SharedPtr<lug::Graphics::Render::Texture> lug::Graphics::Vulkan::Builder::Texture::build(const ::lug::Graphics::Builder::Texture&);

    struct Layer {
        const unsigned char* data{nullptr};
    };

public:
    enum class Type {
        Texture2D,
        CubeMap
    };

public:
    explicit Texture(Renderer& renderer);

    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

    ~Texture();

    /**
     * @brief      Sets the name.
     * @param[in]  name  The name of the mesh.
     */
    void setName(const std::string& name);

    void setType(Type type);

    void setMipLevels(uint32_t mipLevels);

    void setMagFilter(Render::Texture::Filter magFilter);
    void setMinFilter(Render::Texture::Filter minFilter);
    void setMipMapFilter(Render::Texture::Filter mipMapFilter);
    void setWrapS(Render::Texture::WrappingMode wrapS);
    void setWrapT(Render::Texture::WrappingMode wrapT);
    void setWrapW(Render::Texture::WrappingMode wrapW);

    bool addLayer(const std::string& filename, bool hdr = false);
    bool addLayer(uint32_t width, uint32_t height, Render::Texture::Format format, const unsigned char* data = nullptr);

    Resource::SharedPtr<Render::Texture> build();

protected:
    Renderer& _renderer;

    std::string _name;

    Type _type{Type::Texture2D};

    uint32_t _width{0};
    uint32_t _height{0};
    Render::Texture::Format _format{Render::Texture::Format::Undefined};

    uint32_t _mipLevels{1};

    Render::Texture::Filter _magFilter{Render::Texture::Filter::Nearest};
    Render::Texture::Filter _minFilter{Render::Texture::Filter::Nearest};
    Render::Texture::Filter _mipMapFilter{Render::Texture::Filter::Nearest};

    Render::Texture::WrappingMode _wrapS{Render::Texture::WrappingMode::ClampToEdge};
    Render::Texture::WrappingMode _wrapT{Render::Texture::WrappingMode::ClampToEdge};
    Render::Texture::WrappingMode _wrapW{Render::Texture::WrappingMode::ClampToEdge};

    std::vector<Layer> _layers;
};

#include <lug/Graphics/Builder/Texture.inl>

} // Builder
} // Graphics
} // lug
