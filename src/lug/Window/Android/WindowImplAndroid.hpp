#pragma once

#include <lug/Window/Window.hpp>
#include <android/input.h>
#include <android/native_window.h>
#include <android/native_activity.h>

namespace lug {
namespace Window {
namespace priv {


class LUG_WINDOW_API WindowImpl {
public:
    WindowImpl(Window*);
    ~WindowImpl() = default;

    bool init(const Window::InitInfo& initInfo);
    void close();
    bool pollEvent(lug::Window::Event& event);
    ANativeWindow* getWindow();

    LUG_WINDOW_API static std::queue<lug::Window::Event> events;
    LUG_WINDOW_API static AInputQueue* inputQueue;
    LUG_WINDOW_API static ANativeWindow* nativeWindow;
    LUG_WINDOW_API static ANativeActivity* activity;
private:
    Window* _parent{nullptr};
};

} // namespace priv
} // namespace Window
} // namespace lug
