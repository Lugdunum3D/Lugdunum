#pragma once

#include <lug/Window/Window.hpp>
#include <android/input.h>

struct androidApp
{
    std::queue<lug::Window::Event> events;
    AInputQueue* inputQueue;
};

namespace lug {
namespace Window {
namespace priv {


class WindowImpl {
public:
    WindowImpl(Window*);
    ~WindowImpl() = default;

    bool create(const std::string& title, Style style);
    void close();
    bool pollEvent(lug::Window::Event& event);
private:
};

} // namespace priv
} // namespace Window
} // namespace lug