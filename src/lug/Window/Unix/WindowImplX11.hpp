#pragma once

#include <lug/Math/Vector.hpp>
#include <lug/Window/Window.hpp> // I include Window.hpp before Xlib because Xlib is dumb
#include <X11/Xlib.h>

namespace lug {
namespace Window {
namespace priv {

class LUG_WINDOW_API WindowImpl {
public:
    WindowImpl(Window*);
    ~WindowImpl() = default;

    WindowImpl(const WindowImpl&) = delete;
    WindowImpl(WindowImpl&&) = delete;
    WindowImpl& operator=(const WindowImpl&) = delete;
    WindowImpl& operator=(WindowImpl&&) = delete;

    bool init(const Window::InitInfo& initInfo);
    void close();

    bool pollEvent(lug::Window::Event& event);
    void setKeyRepeat(bool state);
    void setMouseCursorVisible(bool visible);
    void setMousePos(const Math::Vec2i& mousePosition);

    Display* getDisplay() const;
    ::Window getWindow() const;

private:
    void setWindowDecorations(Style style);
    bool shouldIgnoreRepeated(XEvent& xEvent);

private:
    Window* _parent{nullptr};

    Display* _display{nullptr};
    ::Window  _window{0};

    // Atoms are really are just ints, so we can initialize them to 0 with {}
    Atom _wmProtocols{};
    Atom _wmDeleteWindow{};
    Atom _wmHints{};

    Cursor _hiddenCursor{0};    ///< Invisible cursor used to hide the pointer

    bool _keyRepeat{true};
};

} // namespace priv
} // namespace Window
} // namespace lug
