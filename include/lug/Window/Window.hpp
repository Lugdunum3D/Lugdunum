#pragma once

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <lug/Math/Vector.hpp>
#include <lug/Window/Event.hpp>
#include <lug/Window/Export.hpp>
#include <lug/Window/Keyboard.hpp>
#include <lug/Window/WindowHandle.hpp>

namespace lug {
namespace Window {

/**
 * @cond HIDDEN_SYMBOLS
 * Disables doxygen
 */

namespace priv {
    class WindowImpl;
} // namespace priv

/**
 * @endcond
 */

/**
 * @brief      The video mode we set for the window we wish to create.
 */
struct VideoMode {
    uint16_t width;
    uint16_t height;
    uint8_t redBits;
    uint8_t greenBits;
    uint8_t blueBits;
    uint16_t refreshRate;
};

/**
 * Defines the style of the window we wish to create.
 */
enum class LUG_WINDOW_API Style : uint16_t {
    None        = 0,
    Titlebar    = 1 << 0,
    Resize      = 1 << 1,
    Close       = 1 << 2,
    Fullscreen  = 1 << 3,

    Default = Titlebar | Resize | Close
};

#include <lug/Window/Style.inl>

/**
 * @brief      Class for window.
 */
class LUG_WINDOW_API Window {
public:
    struct InitInfo {
        uint16_t width;
        uint16_t height;
        std::string title;
        ::lug::Window::Style style;
    };

public:
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;
    virtual ~Window();

    /**
     * @brief      Utility to create a window
     *
     * @param[in]  initInfo   See #InitInfo
     *
     * @return     A pointer to the created window handle. The ownership is transfered to the caller.
     */
    static std::unique_ptr<Window> create(const InitInfo& initInfo);

    /**
     * @brief      Determines if the window is open.
     *
     * @return     True if open, False otherwise.
     */
    bool isOpen() const;

    /**
     * @brief      Checks if an event is available from the window implementation,
     *             and fill it in the @p event parameter.
     *
     * @param[out] event  Event input, to be filled by #pollEvent.
     *
     * @return     True if an event was polled, false otherwise.
     */
    virtual bool pollEvent(lug::Window::Event& event);

    /**
     * @brief      Close the window gracefully.
     */
    void close();

    /**
     * @brief      Enables or disables key repeat.
     *
     * The key repeat is set to false by default (i.e. the default, unpressed, behavior)
     *
     * @param[in]  state  Desired state
     */
    void setKeyRepeat(bool state);

    /**
     * @brief      Determines if a key pressed.
     *
     * @param[in]  key   The key to check.
     *
     * @return     True if the key is pressed, False otherwise.
     */
    bool isKeyPressed(Keyboard::Key key) const;

    /**
    * @brief      Determines if a mouse button pressed.
    *
    * @param[in]  button   The button to check.
    *
    * @return     True if the button is pressed, False otherwise.
    */
    bool isMousePressed(Mouse::Button button) const;

    /**
    * @brief      Retrieves the mouses position
    *
    * @return     Position of the cursor (relative to the window).
    */
    const Math::Vec2i& getMousePos() const;

    /**
     * @brief      Sets the mouse position.
     *
     * @param[in]  mousePosition  The position of the cursor (relative to the window)
     */
    void setMousePos(const Math::Vec2i& mousePosition);

    /**
     * @brief      Gets the window size.
     *
     * @return     The window size.
     */
    Math::Vec2i getWindowSize() const;

    /**
     * @brief      Sets the visibility of the mouse cursor (hide/show)
     *
     * @param[in]  visible  The state
     */
    void setMouseCursorVisible(bool visible);

    GamePadEvent _gamePadState{{0.0f, 0.0f}, {0.0f, 0.0f}};


protected:
    Window();
    bool init(const InitInfo& initInfo);

    /**
     * @brief       Inits every key in @p _keyState to false
     */
    void initKeyState();

    /**
    * @brief        Inits every button in @p _mouseState to false
    */
    void initMouseState();

    /**
     * Internal representation of the platform specific implementation.
     */
    priv::WindowImpl* _impl{nullptr};

    /**
     * Default video mode.
     */
    VideoMode _mode{800, 600, 8, 8, 8, 60};

    /**
     * Map to store the current state of the keys, used by #isKeyPressed.
     */
    std::unordered_map<Keyboard::Key, bool> _keyState;

    /**
    * Map to store the current state of the mouse buttons, used by #isMousePressed.
    */
    std::unordered_map<Mouse::Button, bool> _mouseState;

    /**
    * Used to store the mouse's position, used by #getMousePos.
    */
    Math::Vec2i _mousePosition{0, 0};
    friend lug::Window::priv::WindowImpl;
};

} // namespace Window
} // namespace lug
