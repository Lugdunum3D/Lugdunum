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

    enum class Format : uint8_t {
        Undefined,
        R8G8B8A8_UNORM,
        R16G16_SFLOAT,
        R16G16B16_SFLOAT,
        R32G32B32A32_SFLOAT
    };

public:
    Texture(const std::string& name);

    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

    virtual ~Texture() = default;

    /**
     * @brief      Gets the width.
     *
     * @return     The width.
     */
    uint32_t getWidth() const;
    /**
     * @brief      Gets the height.
     *
     * @return     The height.
     */
    uint32_t getHeight() const;
    /**
     * @brief      Gets the format.
     *
     * @return     The format.
     */
    Render::Texture::Format getFormat() const;

    /**
     * @brief      Gets the magnitude filter.
     *
     * @return     The magnitude filter.
     */
    Render::Texture::Filter getMagFilter() const;
    /**
     * @brief      Gets the minimum filter.
     *
     * @return     The minimum filter.
     */
    Render::Texture::Filter getMinFilter() const;
    /**
     * @brief      Gets the mip map filter.
     *
     * @return     The mip map filter.
     */
    Render::Texture::Filter getMipMapFilter() const;
    /**
     * @brief      Gets the wrap s.
     *
     * @return     The wrap s.
     */
    Render::Texture::WrappingMode getWrapS() const;
    /**
     * @brief      Gets the wrap t.
     *
     * @return     The wrap t.
     */
    Render::Texture::WrappingMode getWrapT() const;

    static size_t formatToSize(Render::Texture::Format format);

private:
    uint32_t _width{0};
    uint32_t _height{0};
    Render::Texture::Format _format;

    Render::Texture::Filter _magFilter;
    Render::Texture::Filter _minFilter;
    Render::Texture::Filter _mipMapFilter;

    Render::Texture::WrappingMode _wrapS;
    Render::Texture::WrappingMode _wrapT;
};

#include <lug/Graphics/Render/Texture.inl>

} // Render
} // Graphics
} // lug
