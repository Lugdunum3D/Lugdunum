#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Resource.hpp>

namespace lug {
namespace Graphics {

// For friend
namespace Builder {
class Texture;
} // Builder

namespace Render {

class LUG_GRAPHICS_API Texture : public Resource {
    friend class ::lug::Graphics::Builder::Texture;

public:
    enum class Filter : uint8_t {
        Nearest,
        Linear
    };

    enum class WrappingMode : uint8_t {
        ClampToEdge,
        MirroredRepeat,
        Repeat
    };

public:
    Texture(const std::string& name);

    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

    virtual ~Texture() = default;

    void setWidth(uint32_t width);
    void setHeight(uint32_t height);

    Render::Texture::Filter getMagFilter() const;
    Render::Texture::Filter getMinFilter() const;
    Render::Texture::Filter getMipMapFilter() const;
    Render::Texture::WrappingMode getWrapS() const;
    Render::Texture::WrappingMode getWrapT() const;
    uint32_t getWidth() const;
    uint32_t getHeight() const;

private:
    Render::Texture::Filter _magFilter;
    Render::Texture::Filter _minFilter;
    Render::Texture::Filter _mipMapFilter;

    Render::Texture::WrappingMode _wrapS;
    Render::Texture::WrappingMode _wrapT;

    uint32_t _width{0};
    uint32_t _height{0};
};

#include <lug/Graphics/Render/Texture.inl>

} // Render
} // Graphics
} // lug
