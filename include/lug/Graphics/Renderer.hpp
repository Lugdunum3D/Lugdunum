#pragma once

#include <cstdint>
#include <set>
#include <string>

#include <lug/Core/Version.hpp>
#include <lug/Graphics/Module.hpp>
#include <lug/Graphics/Render/Window.hpp>
#include <lug/Graphics/Render/Technique/Type.hpp>
#include <lug/Graphics/ResourceManager.hpp>

namespace lug {
namespace Graphics {

class Graphics;

class LUG_GRAPHICS_API Renderer {
public:
    enum class Type : uint8_t {
        Vulkan
    };

    enum class DisplayMode : uint8_t {
        Full,
        Albedo,
        Normal,
        Metallic,
        Roughness,
        AmbientOcclusion,
        AmbientOcclusionRoughnessMetallic,
        Emissive
    };

    enum class Antialiasing : uint8_t {
        NoAA,
        MSAA2X,
        MSAA4X,
        MSAA8X,
        MSAA16X
    };

    struct InitInfo {
        std::string shadersRoot;
        Render::Technique::Type renderTechnique;
        DisplayMode displayMode{DisplayMode::Full};
        Antialiasing antialiasing{Antialiasing::NoAA};
#if defined(LUG_SYSTEM_ANDROID)
        bool bloomEnabled{false};
#else
        bool bloomEnabled{true};
#endif
    };

public:
    Renderer(Graphics& graphics, Type type);

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    virtual ~Renderer() = default;

    virtual bool beginInit(const std::string& appName, const Core::Version& appVersion, const InitInfo& initInfo) = 0;
    virtual bool finishInit() = 0;

    virtual bool beginFrame(const lug::System::Time& elapsedTime) = 0;
    virtual bool endFrame() = 0;

    virtual Render::Window* createWindow(Render::Window::InitInfo& initInfo) = 0;
    virtual Render::Window* getWindow() = 0;

    const InitInfo& getInfo() const;
    Type getType() const;

    const DisplayMode& getDisplayMode() const;
    void setDisplayMode(DisplayMode displayMode);

    const Antialiasing& getAntialiasing() const;
    void setAntialiasing(Antialiasing antialiasing);

    const bool& isBloomEnabled() const;
    void isBloomEnabled(bool enabled);

    ResourceManager* getResourceManager() const;

protected:
    Graphics& _graphics;
    Type _type;
    DisplayMode _displayMode;
    Antialiasing _antialiasing;
    bool _bloomEnabled;
    InitInfo _initInfo;
    std::unique_ptr<ResourceManager> _resourceManager{nullptr};
};

#include <lug/Graphics/Renderer.inl>

} // Graphics
} // lug
