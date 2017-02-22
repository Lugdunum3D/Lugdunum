#pragma once

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
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
    Window(const Window&) = delete;
    Window(Window&&);
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&);
    ~Window();

    /**
     * @brief      Utility to create a window
     *
     * @param[in]  width   The width desired
     * @param[in]  height  The height desired
     * @param[in]  title   The title of the window
     * @param[in]  style   The style to apply
     *
     * @return     A pointer to the created window handle. The ownership is transfered to the caller.
     */
    static std::unique_ptr<Window> create(uint16_t width, uint16_t height, const std::string& title, Style style);

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
    bool pollEvent(lug::Window::Event& event);

    /**
     * @brief      Close the window gracefully.
     */
    void close();

    /**
     * @brief      Enables or disables key repeat.
     *
     * The key repeat is set to false by default (i.e. the default, untouched, behaviour)
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

private:
    Window();

    bool createWindow(uint16_t width, uint16_t height, const std::string& title, Style style);

    /**
     * @brief      Inits every key in @p _keyState to false
     */
    void initKeyState();

    /**
     * Internal representation of the platform specific implementation.
     */
    priv::WindowImpl* _impl{nullptr};

    /**
     * Default videomode.
     */
    VideoMode _mode{800, 600, 8, 8, 8, 60};

    /**
     * Map to store the current state of the keys, used by #isKeyPressed.
     */
    std::unordered_map<Keyboard::Key, bool> _keyState;

    friend lug::Window::priv::WindowImpl;
};

} // namespace Window
} // namespace lug
