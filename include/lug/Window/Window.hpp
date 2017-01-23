#pragma once

#include <memory>
#include <queue>
#include <string>
#include <lug/Window/Event.hpp>
#include <lug/Window/Export.hpp>
#include <lug/Window/WindowHandle.hpp>

namespace lug {
namespace Window {

namespace priv {
    class WindowImpl;
} // namespace priv

// The video mode we set for the window we wish to create
struct VideoMode
{
    uint16_t width;
    uint16_t height;
    uint8_t redBits;
    uint8_t greenBits;
    uint8_t blueBits;
    uint16_t refreshRate;
};

// This defines the style of the window we wish to create
enum class LUG_WINDOW_API Style : uint16_t {
    None = 0,
    Titlebar = 1 << 0,
    Resize = 1 << 1,
    Close = 1 << 2,
    Fullscreen = 1 << 3,

    Default = Titlebar | Resize | Close
};

#include <lug/Window/Style.inl>

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

    static std::unique_ptr<Window> create(const InitInfo& initInfo);

    bool isOpen() const;
    virtual bool pollEvent(lug::Window::Event&);
    void close();

protected:
    Window();

    priv::WindowImpl* _impl{nullptr};
    VideoMode _mode{800, 600, 8, 8, 8, 60};

    friend lug::Window::priv::WindowImpl;

protected:
    bool init(const InitInfo& initInfo);
};

} // Window
} // lug
