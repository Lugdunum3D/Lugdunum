#pragma once

#include <algorithm>
#include <memory>
#include <set>
#include <lug/Core/Application.hpp>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Module.hpp>
#include <lug/Graphics/Renderer.hpp>

namespace lug {
namespace Graphics {

class LUG_GRAPHICS_API Graphics {
public:
    explicit Graphics(const Core::Application::Info& appInfo);

    Graphics(const Graphics&) = delete;
    Graphics(Graphics&&) = delete;

    Graphics& operator=(const Graphics&) = delete;
    Graphics& operator=(Graphics&&) = delete;

    ~Graphics() = default;

    bool init();

    void addModule(Module::Type type);
    void addOptionalModule(Module::Type type);

    void removeModule(Module::Type type);
    void removeOptionalModule(Module::Type type);

    const std::set<Module::Type>& getMandatoryModules() const;
    const std::set<Module::Type>& getOptionalModules() const;

    bool isModuleLoaded(Module::Type type) const;
    const std::set<Module::Type>& getLoadedModules() const;

    // TODO: setRendererType, getRendererType
    // TODO: getRenderer

    const Core::Application::Info& getAppInfo() const;

    Renderer* getRenderer() const;

private:
    Core::Application::Info _appInfo;

    std::set<Module::Type> _mandatoryModules{Module::Type::Core};
    std::set<Module::Type> _optionalModules{};
    std::set<Module::Type> _loadedModules{};

    Renderer::Type _rendererType{Renderer::Type::Vulkan};
    std::unique_ptr<Renderer> _renderer{nullptr};
};

#include <lug/Graphics/Graphics.inl>

} // Graphics
} // lug
