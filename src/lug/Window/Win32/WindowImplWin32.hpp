#pragma once

#include <Windows.h>
#include <lug/Window/Window.hpp>

namespace lug {
namespace Window {
namespace priv {

class WindowImpl {
public:
    WindowImpl(Window*);
    ~WindowImpl();

    WindowImpl(const WindowImpl&) = delete;
    WindowImpl(WindowImpl&&) = delete;
    WindowImpl& operator=(const WindowImpl&) = delete;
    WindowImpl& operator=(WindowImpl&&) = delete;

    bool create(const std::string& title, Style style);
    void close();

    bool pollEvent(lug::Window::Event&);
    void setKeyRepeat(bool state);

private:
    void processWindowEvents(UINT message, WPARAM wParam, LPARAM lParam);
    void registerWindow() const;
    bool activateFullscreen();
    Keyboard::Key getKeyCode(WPARAM wParam, LPARAM lParam);
    void configKeyEvent(KeyEvent& key, WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK onEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

private:
    const wchar_t* className{L"LUG_Window"};

    Window* _parent{nullptr};
    std::queue<lug::Window::Event> _events;

    HWND _handle{nullptr};         // Win32 handle of the window
    LONG_PTR _callback{0};         // Stores the original event callback function of the control
    HCURSOR _cursor{nullptr};      // The system cursor to display into the window
    HICON _icon{nullptr};          // Custom icon assigned to the window
    bool _fullscreen{false};       // Is the window fullscreen?

    static uint8_t windowCount;
    static lug::Window::priv::WindowImpl*  fullscreenWindow;
    bool _keyRepeat{true};
};

} // namespace priv
} // namespace Window
} // namespace lug
