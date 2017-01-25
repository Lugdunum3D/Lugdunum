#pragma once

#include <cstdint>
#include <lug/Core/Export.hpp>
#include <lug/Graphics/Graphics.hpp>
#include <lug/Window/Window.hpp>

namespace lug {
namespace Core {

class LUG_CORE_API Application {
public:
    struct Version {
        uint32_t major; // 10 bits
        uint32_t minor; // 10 bits
        uint32_t patch; // 12 bits

        uint32_t createVersion() const;

        operator uint32_t() const {
            return createVersion();
        }
    };

    struct Info {
        const char* name;
        Version version;
    };

public:
    explicit Application(const Info& info);

    Application(const Application&) = delete;
    Application(Application&&) = delete;

    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    virtual ~Application() = default;

    const Application::Info& getInfo() const;

    lug::Graphics::Graphics& getGraphics();
    const lug::Graphics::Graphics& getGraphics() const;

    lug::Graphics::RenderWindow* getWindow();
    const lug::Graphics::RenderWindow* getWindow() const;

    void setRenderWindowInfo(lug::Graphics::RenderWindow::InitInfo& initInfo);
    const lug::Graphics::RenderWindow::InitInfo& getRenderWindowInfo() const;
    lug::Graphics::RenderWindow::InitInfo& getRenderWindowInfo();

    void setGraphicsInfo(const lug::Graphics::Graphics::InitInfo& initInfo);
    const lug::Graphics::Graphics::InitInfo& getGraphicsInfo() const;
    lug::Graphics::Graphics::InitInfo& getGraphicsInfo();

    bool init(int argc, char* argv[]);
    bool run();
    void close();

    virtual void onEvent(const lug::Window::Event& event) = 0;
    virtual void onFrame() = 0;

private:
    bool beginFrame();
    bool endFrame();

private:
    Info _info;
    bool _closed{false};

    // Init infos
    lug::Graphics::Graphics::InitInfo _graphicsInitInfo{
        lug::Graphics::Renderer::Type::Vulkan,      // type
        {                                           // rendererInitInfo
            {                                       // mandatoryModules
                lug::Graphics::Module::Type::Core
            },
            {}                                      // optionalModules
        }
    };

    lug::Graphics::RenderWindow::InitInfo _renderWindowInitInfo{
        {                               // windowInitInfo
            800,                        // width
            600,                        // height
            "Lugdunum - Default title", // title
            lug::Window::Style::Default // style
        },

        {}                              // renderViewsInitInfo
    };

    lug::Graphics::Graphics _graphics;
    lug::Graphics::RenderWindow* _window{nullptr};
};

#include <lug/Core/Application.inl>

} // Core
} // lug
