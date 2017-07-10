#pragma once

#include <cstdint>

#include <lug/Core/Export.hpp>
#include <lug/Core/Version.hpp>
#include <lug/Graphics/Graphics.hpp>
#include <lug/Graphics/Render/Window.hpp>
#include <lug/System/Time.hpp>

namespace lug {
namespace Core {

/**
 * @brief      Class for application.
 *
 *             Root class of the Engine. The user has to inherit from this class
 *             and implement #onFrame and #onEvent.
 */
class LUG_CORE_API Application {
public:
    struct Info {
        std::string name;
        Version version;
    };

public:
    explicit Application(const Info& info);

    Application(const Application&) = delete;
    Application(Application&&) = delete;

    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    virtual ~Application() = default;

    /**
     * @brief      Gets the application's information.
     *
     * @return     The application's information.
     */
    const Application::Info& getInfo() const;

    /**
     * @brief      Gets the lug::Graphics::Graphics component of the Application.
     *
     * @return     The lug::Graphics::Graphics component.
     */
    lug::Graphics::Graphics& getGraphics();
    const lug::Graphics::Graphics& getGraphics() const;

    /**
     * @brief      Gets the Window attached to the Application.
     *
     * @return     The lug::Graphics::Render::Window.
     */
    lug::Graphics::Render::Window* getWindow();
    const lug::Graphics::Render::Window* getWindow() const;

    /**
     * @brief      Sets the render window information.
     *
     * @param      initInfo  The initialize information.
     */
    void setRenderWindowInfo(lug::Graphics::Render::Window::InitInfo& initInfo);

    /**
     * @brief      Gets the render window information.
     *
     * @return     The render window information.
     */
    const lug::Graphics::Render::Window::InitInfo& getRenderWindowInfo() const;
    lug::Graphics::Render::Window::InitInfo& getRenderWindowInfo();

    /**
     * @brief      Sets the graphics information.
     *
     * @param[in]  initInfo  A reference to the RenderWindow initialize information.
     */
    void setGraphicsInfo(const lug::Graphics::Graphics::InitInfo& initInfo);

    /**
     * @brief      Gets the graphics information.
     *
     * @return     A reference the Graphics initialize information.
     */
    const lug::Graphics::Graphics::InitInfo& getGraphicsInfo() const;
    lug::Graphics::Graphics::InitInfo& getGraphicsInfo();

    /**
     * @brief      Init the application with the informations filled in the lug::Graphics::Graphics::InitInfo
     *             and lug::Graphics::RenderWindow::InitInfo structures.
     *
     *             The lug::Graphics::Graphics::InitInfo structure can be modified by calling #getGraphicsInfo or #setGraphicsInfo. @n
     *             The lug::Graphics::RenderWindow::InitInfo structure can be modified by calling #getRenderWindowInfo or #setRenderWindowInfo.
     *
     * @param[in]  argc  The argc argument as received from the main function.
     * @param[in]  argv  The argv argument as received from the main function.
     *
     * @return     Whether the application was successfully initialized.
     */
    bool init(int argc, char* argv[]);

    /**
     * @brief      Begin the initialization of the application with the informations filled in the
     *             lug::Graphics::Graphics::InitInfo structure.
     *
     *             The lug::Graphics::Graphics::InitInfo structure can be modified by calling #getGraphicsInfo or #setGraphicsInfo. @n
     *
     * @param[in]  argc  The argc argument as received from the main function.
     * @param[in]  argv  The argv argument as received from the main function.
     *
     * @return     Whether the application was successfully initialized.
     */
    bool beginInit(int argc, char* argv[]);

    /**
     * @brief      Finish the initialization of the application with the informations filled in the
     *             lug::Graphics::RenderWindow::InitInfo structure.
     *
     *             The lug::Graphics::RenderWindow::InitInfo structure can be modified by calling #getRenderWindowInfo or #setRenderWindowInfo.
     *
     * @return     Whether the application was successfully initialized.
     */
    bool finishInit();

    /**
     * @brief      Run the application.
     *
     *             Launches an infinite loop (i.e. it does not exit immediately) and runs the application in the foreground.
     *
     * @return     Whether the application exited successfully.
     */
    bool run();

    /**
     * @brief      Close the application.
     *
     *             This can be called in #onEvent or #onFrame.
     */
    void close();

    /**
     * @brief      Override this function to handle an event.
     *
     *             This function will be called by the engine for each polled event.
     *
     * @param[in]  event  The event that was polled.
     */
    virtual void onEvent(const lug::Window::Event& event) = 0;

    /**
     * @brief      Override this function to handle a frame.
     *
     *             This function will be called by the engine at each frame.
     */
    virtual void onFrame(const System::Time& elapsedTime) = 0;

private:
    bool beginFrame(const lug::System::Time& elapsedTime);
    bool endFrame();

private:
    Info _info;
    bool _closed{false};


    lug::Graphics::Graphics::InitInfo _graphicsInitInfo{
        lug::Graphics::Renderer::Type::Vulkan,              // type
        {                                                   // rendererInitInfo
            "shaders/",                                     // shaders root
            lug::Graphics::Render::Technique::Type::Forward // renderTechnique
        },
        {                                                   // mandatoryModules
            lug::Graphics::Module::Type::Core
        },
        {},                                                 // optionalModules
    };

    lug::Graphics::Render::Window::InitInfo _renderWindowInitInfo{
        {                               // windowInitInfo
            800,                        // width
            600,                        // height
            "Lugdunum - Default title", // title
            lug::Window::Style::Default // style
        },

        {}                              // renderViewsInitInfo
    };

    lug::Graphics::Render::Window* _window{nullptr};

protected:
    lug::Graphics::Graphics _graphics;
};


#include <lug/Core/Application.inl>

} // Core
} // lug
