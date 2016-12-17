#pragma once

#include <lug/Graphic/Export.hpp>
#include <lug/Graphic/Renderer.hpp>
#include <lug/Graphic/Vulkan/Instance.hpp>
#include <lug/Graphic/Vulkan/Loader.hpp>
#include <lug/Graphic/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphic {

class Graphic;

namespace Vulkan {

class LUG_GRAPHIC_API Renderer final : public ::lug::Graphic::Renderer {
public:
    Renderer(Graphic&);

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    ~Renderer() = default;

    void init() override final;

private:
    Loader _loader; // Need to be at the beginning, we don't want to unload Vulkan functions too early
    Graphic& _graphic;

    Instance _instance;
    InstanceInfo _instanceInfo;
};

} // Vulkan
} // Graphic
} // lug
