#pragma once

#include <memory>
#include <queue>
#include <lug/Window/Event.hpp>
#include <lug/Window/Export.hpp>
#include <lug/Window/WindowHandle.hpp>

namespace lug {
namespace Window {
    
namespace priv {
    class WindowImpl;
} // namespace priv

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
    Window(const Window&) = delete;
    Window(Window&&);
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&);
    ~Window();
    
    bool isOpen() const;
    bool pollEvent(Event& event);
    void close();

    static std::unique_ptr<Window> create(uint16_t width, uint16_t height, const std::string& title, Style style);

private:
    Window();
    
    bool createWindow(uint16_t width, uint16_t height, const std::string& title, Style style);

    priv::WindowImpl* _impl;
    std::queue<Event> _events;
};

} // namespace Window
} // namespace lug
