#pragma once

#include <lug/Window/Window.hpp>

namespace lug {
namespace Window {
namespace priv {
    
class WindowImpl {
public:
    WindowImpl() = default;
    ~WindowImpl() = default;

    WindowImpl(const WindowImpl&) = delete;
    WindowImpl(WindowImpl&&) = delete;
    WindowImpl& operator=(const WindowImpl&) = delete;
    WindowImpl& operator=(WindowImpl&&) = delete;

    bool create(uint16_t width, uint16_t height, const std::string& title, Style style);
    void close();
    void processEvents(std::queue<lug::Window::Event>& events);
};

} // namespace priv
} // namespace Window
} // namespace lug
