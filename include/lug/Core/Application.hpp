#pragma once

#include <cstdint>
#include <lug/Core/Export.hpp>
#include <lug/Graphics/Graphics.hpp>
#include <lug/Graphics/RenderWindow.hpp>

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
    /**
     * @brief      Stores the version of the Application
     */
    struct Version {
        uint32_t major; /** 10 bits */
        uint32_t minor; /** 10 bits */
        uint32_t patch; /** 12 bits */

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

    /**
     * @brief      Gets the application's information.
     *
     * @return     The applicaton's information.
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
     * @return     The lug::Graphics::RenderWindow.
     */
    lug::Graphics::RenderWindow* getWindow();
    const lug::Graphics::RenderWindow* getWindow() const;

    /**
     * @brief      Sets the render window information.
     *
     * @param      initInfo  The initialize information.
     */
    void setRenderWindowInfo(lug::Graphics::RenderWindow::InitInfo& initInfo);

    /**
     * @brief      Gets the render window information.
     *
     * @return     The render window information.
     */
    const lug::Graphics::RenderWindow::InitInfo& getRenderWindowInfo() const;
    lug::Graphics::RenderWindow::InitInfo& getRenderWindowInfo();

    /**
     * @brief      Sets the graphics information.
     *
     * @param[in]  initInfo  A refenrence to the RenderWindow initialize information.
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
     * @return     Wether the application was successfully initialized.
     */
    virtual bool init(int argc, char* argv[]);

    /**
     * @brief      Run the application.
     *
     *             Launches an infinite loop (i.e. it does not exit immediately) and runs the application in the foreground.
     *
     * @return     Wether the application exited successfully.
     */
    bool run();

    /**
     * @brief      Close the application.
     *
     *             This can be called in #onEvent or #onFrame.
     */
    void close();

    /**
     * @brief      Overide this function to hanle an event.
     *
     *             This function will be called by the engine for each polled event.
     *
     * @param[in]  event  The event that was polled.
     */
    virtual void onEvent(const lug::Window::Event& event) = 0;

    /**
     * @brief      Overide this function to handle a frame.
     *
     *             This function will be called by the engine at each frame.
     */
    virtual void onFrame(float elapsedTime) = 0;

private:
    bool beginFrame();
    bool endFrame();

private:
    Info _info;
    bool _closed{false};


    lug::Graphics::Graphics::InitInfo _graphicsInitInfo{
        lug::Graphics::Renderer::Type::Vulkan,      // type
        {                                           // rendererInitInfo
            {                                       // mandatoryModules
                lug::Graphics::Module::Type::Core
            },
            {},                                     // optionalModules
            true                                    // use dedicaced GPU
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

    lug::Graphics::RenderWindow* _window{nullptr};

protected:
    lug::Graphics::Graphics _graphics;
};


#include <lug/Core/Application.inl>

} // Core
} // lug
