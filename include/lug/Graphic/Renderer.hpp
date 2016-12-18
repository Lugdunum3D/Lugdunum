#pragma once

#include <cstdint>
#include <set>
#include <lug/Graphic/Module.hpp>

namespace lug {
namespace Graphic {

class LUG_GRAPHIC_API Renderer {
public:
    enum class Type : uint8_t {
        Vulkan
    };

public:
    Renderer() = default;

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    virtual ~Renderer() = default;

    virtual std::set<Module::Type> init() = 0;
};

} // Graphic
} // lug
