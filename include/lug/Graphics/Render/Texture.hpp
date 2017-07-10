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

public:
    Texture(const std::string& name);

    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

    virtual ~Texture() = default;
};

} // Render
} // Graphics
} // lug
