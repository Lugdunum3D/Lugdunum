#include <lug/Window/Unix/WindowImplX11.hpp>
#include <cstring>
#include <string>
#include <queue>
#include <lug/System/Logger/Logger.hpp>
#include <lug/Window/Unix/WmHints.hpp>
#include <X11/Xlib.h>
#include <lug/Window/Event.hpp>

namespace lug {
namespace Window {
namespace priv {

WindowImpl::WindowImpl(Window* win) : _parent{win} {}

bool WindowImpl::init(const Window::InitInfo& initInfo) {
    _display = XOpenDisplay(nullptr);

    if (_display == nullptr) {
        return false;
    }

    int screen = DefaultScreen(_display);
    ::Window parent = RootWindow(_display, screen);

    // The default color
    uint32_t blackColor = BlackPixel(_display, DefaultScreen(_display));

    _window = XCreateSimpleWindow(_display, parent, 0, 0, initInfo.width, initInfo.height, 90, 2, blackColor);

    if (!_window) {
        XCloseDisplay(_display);
        _display = nullptr;
        return false;
    }

    XSelectInput(_display, _window, ExposureMask |
                                    StructureNotifyMask |
                                    KeyPressMask |
                                    KeyReleaseMask |
                                    ButtonPressMask |
                                    ButtonReleaseMask |
                                    EnterWindowMask |
                                    LeaveWindowMask |
                                    PointerMotionMask);
    XStoreName(_display, _window, initInfo.title.c_str());
    XMapWindow(_display, _window);

    _wmProtocols = XInternAtom(_display, "WM_PROTOCOLS", false);
    _wmDeleteWindow = XInternAtom(_display, "WM_DELETE_WINDOW", false);
    _wmHints = XInternAtom(_display, "_MOTIF_WM_HINTS", false);

    XSetWMProtocols(_display, _window, &_wmDeleteWindow, 1);
    setWindowDecorations(initInfo.style);

    return true;
}

void WindowImpl::close() {
    if (_display != nullptr) {
        XDestroyWindow(_display, _window);
        XCloseDisplay(_display);
        _display = nullptr;
        _window = 0;
    }
}

static Mouse::Button buttonCodeToLugButton(unsigned int buttonCode) {
    switch (buttonCode) {
        case Button1:       return Mouse::Button::Left;
        case Button2:       return Mouse::Button::Middle;
        case Button3:       return Mouse::Button::Right;
        default:            return Mouse::Button::Unknown;
    }
}

static Keyboard::Key keysymToLugKey(KeySym key) {
    switch (key) {
        case XK_F1:
            return Keyboard::Key::F1;                                       // The F1 key
        case XK_F2:
            return Keyboard::Key::F2;                                       // The F2 key
        case XK_F3:
            return Keyboard::Key::F3;                                       // The F3 key
        case XK_F4:
            return Keyboard::Key::F4;                                       // The F4 key
        case XK_F5:
            return Keyboard::Key::F5;                                       // The F5 key
        case XK_F6:
            return Keyboard::Key::F6;                                       // The F6 key
        case XK_F7:
            return Keyboard::Key::F7;                                       // The F7 key
        case XK_F8:
            return Keyboard::Key::F8;                                       // The F8 key
        case XK_F9:
            return Keyboard::Key::F9;                                       // The F9 key
        case XK_F10:
            return Keyboard::Key::F10;                                      // The F10 key
        case XK_F11:
            return Keyboard::Key::F11;                                      // The F11 key
        case XK_F12:
            return Keyboard::Key::F12;                                      // The F12 key
        case XK_F13:
            return Keyboard::Key::F13;                                      // The F13 key
        case XK_F14:
            return Keyboard::Key::F14;                                      // The F14 key
        case XK_F15:
            return Keyboard::Key::F15;                                      // The F15 key

        case XK_a:
            return Keyboard::Key::A;                                        // The A key
        case XK_b:
            return Keyboard::Key::B;                                        // The B key
        case XK_c:
            return Keyboard::Key::C;                                        // The C key
        case XK_d:
            return Keyboard::Key::D;                                        // The D key
        case XK_e:
            return Keyboard::Key::E;                                        // The E key
        case XK_f:
            return Keyboard::Key::F;                                        // The F key
        case XK_g:
            return Keyboard::Key::G;                                        // The G key
        case XK_h:
            return Keyboard::Key::H;                                        // The H key
        case XK_i:
            return Keyboard::Key::I;                                        // The I key
        case XK_j:
            return Keyboard::Key::J;                                        // The J key
        case XK_k:
            return Keyboard::Key::K;                                        // The K key
        case XK_l:
            return Keyboard::Key::L;                                        // The L key
        case XK_m:
            return Keyboard::Key::M;                                        // The M key
        case XK_n:
            return Keyboard::Key::N;                                        // The N key
        case XK_o:
            return Keyboard::Key::O;                                        // The O key
        case XK_p:
            return Keyboard::Key::P;                                        // The P key
        case XK_q:
            return Keyboard::Key::Q;                                        // The Q key
        case XK_r:
            return Keyboard::Key::R;                                        // The R key
        case XK_s:
            return Keyboard::Key::S;                                        // The S key
        case XK_t:
            return Keyboard::Key::T;                                        // The T key
        case XK_u:
            return Keyboard::Key::U;                                        // The U key
        case XK_v:
            return Keyboard::Key::V;                                        // The V key
        case XK_w:
            return Keyboard::Key::W;                                        // The W key
        case XK_x:
            return Keyboard::Key::X;                                        // The X key
        case XK_y:
            return Keyboard::Key::Y;                                        // The Y key
        case XK_z:
            return Keyboard::Key::Z;                                        // The Z key

        case XK_0:
            return Keyboard::Key::Num0;                                     // The 0 key
        case XK_1:
            return Keyboard::Key::Num1;                                     // The 1 key
        case XK_2:
            return Keyboard::Key::Num2;                                     // The 2 key
        case XK_3:
            return Keyboard::Key::Num3;                                     // The 3 key
        case XK_4:
            return Keyboard::Key::Num4;                                     // The 4 key
        case XK_5:
            return Keyboard::Key::Num5;                                     // The 5 key
        case XK_6:
            return Keyboard::Key::Num6;                                     // The 6 key
        case XK_7:
            return Keyboard::Key::Num7;                                     // The 7 key
        case XK_8:
            return Keyboard::Key::Num8;                                     // The 8 key
        case XK_9:
            return Keyboard::Key::Num9;                                     // The 9 key

        case XK_Escape:
            return Keyboard::Key::Escape;                                   // The Escape key

        case XK_Control_L:
            return Keyboard::Key::LControl;                                 // The left Control key
        case XK_Shift_L:
            return Keyboard::Key::LShift;                                   // The left Shift key
        case XK_Alt_L:
            return Keyboard::Key::LAlt;                                     // The left Alt key
        case XK_Super_L:
            return Keyboard::Key::LSystem;                                  // The left OS specific key: window (Windows and Linux), apple (MacOS X), ...

        case XK_Control_R:
            return Keyboard::Key::RControl;                                 // The right Control key
        case XK_Shift_R:
            return Keyboard::Key::RShift;                                   // The right Shift key
        case XK_Alt_R:
            return Keyboard::Key::RAlt;                                     // The right Alt key
        case XK_Super_R:
            return Keyboard::Key::RSystem;                                  // The right OS specific key: window (Windows and Linux), apple (MacOS X), ...

        case XK_Menu:
            return Keyboard::Key::Menu;                                     // The Menu key
        case XK_bracketleft:
            return Keyboard::Key::LBracket;                                 // The [ key
        case XK_bracketright:
            return Keyboard::Key::RBracket;                                 // The ] key
        case XK_semicolon:
            return Keyboard::Key::SemiColon;                                // The , key
        case XK_comma:
            return Keyboard::Key::Comma;                                    // The , key
        case XK_period:
            return Keyboard::Key::Period;                                   // The . key
        case XK_apostrophe:
            return Keyboard::Key::Quote;                                    // The ' key
        case XK_slash:
            return Keyboard::Key::Slash;                                    // The / key
        case XK_backslash:
            return Keyboard::Key::BackSlash;                                // The \ key
        case XK_grave:
            return Keyboard::Key::Tilde;                                    // The ~ key
        case XK_equal:
            return Keyboard::Key::Equal;                                    // The = key
        case XK_minus:
            return Keyboard::Key::Dash;                                     // The - key
        case XK_space:
            return Keyboard::Key::Space;                                    // The Space key
        case XK_Return:
            return Keyboard::Key::Return;                                   // The Return key

        case XK_twosuperior:
            return Keyboard::Key::Twosuperior;                              // The ² key
        case XK_ampersand:
            return Keyboard::Key::Ampersand;                                // The & key
        case XK_eacute:
            return Keyboard::Key::Eacute;                                   // The é key
        case XK_quotedbl:
            return Keyboard::Key::QuoteDouble;                              // The " key
        case XK_parenleft:
            return Keyboard::Key::LParen;                                   // The ( key
        case XK_egrave:
            return Keyboard::Key::Egrave;                                   // The è key
        case XK_underscore:
            return Keyboard::Key::Underscore;                               // The _ key
        case XK_ccedilla:
            return Keyboard::Key::Ccedilla;                                 // The ç key
        case XK_agrave:
            return Keyboard::Key::Agrave;                                   // The à key
        case XK_parenright:
            return Keyboard::Key::RParen;                                   // The ) key
        case XK_dead_circumflex:
            return Keyboard::Key::DeadCircumflex;                           // The ^ key (dead variant)
        case XK_ugrave:
            return Keyboard::Key::Ugrave;                                   // The ù key
        case XK_asterisk:
            return Keyboard::Key::Asterisk;                                 // The * key
        case XK_dollar:
            return Keyboard::Key::Dollar;                                   // The $ key
        case XK_colon:
            return Keyboard::Key::Colon;                                    // The : key
        case XK_exclam:
            return Keyboard::Key::Exclam;                                   // The ! key
        case XK_less:
            return Keyboard::Key::Less;                                     // The < key
        case XK_greater:
            return Keyboard::Key::Greater;                                  // The > key

        // Note: the keys for the keypad below are in the numlock off state, i.e. 8 => KP_Up
        // TODO: It would be nice to actually find a good solution to this problem.
        case XK_KP_Enter:
            return Keyboard::Key::Return;                                   // The Return key (from the keypad)
        case XK_KP_Add:
            return Keyboard::Key::Add;                                      // The + key
        case XK_KP_Subtract:
            return Keyboard::Key::Subtract;                                 // The - key
        case XK_KP_Multiply:
            return Keyboard::Key::Multiply;                                 // The * key
        case XK_KP_Divide:
            return Keyboard::Key::Divide;                                   // The / key
        case XK_KP_Delete:
            return Keyboard::Key::Period;                                   // The . key (from the keypad)

        case XK_KP_Insert:
            return Keyboard::Key::Numpad0;                                  // The numpad 0 key
        case XK_KP_End:
            return Keyboard::Key::Numpad1;                                  // The numpad 1 key
        case XK_KP_Down:
            return Keyboard::Key::Numpad2;                                  // The numpad 2 key
        case XK_KP_Page_Down:
            return Keyboard::Key::Numpad3;                                  // The numpad 3 key
        case XK_KP_Left:
            return Keyboard::Key::Numpad4;                                  // The numpad 4 key
        case XK_KP_Begin:
            return Keyboard::Key::Numpad5;                                  // The numpad 5 key
        case XK_KP_Right:
            return Keyboard::Key::Numpad6;                                  // The numpad 6 key
        case XK_KP_Home:
            return Keyboard::Key::Numpad7;                                  // The numpad 7 key
        case XK_KP_Up:
            return Keyboard::Key::Numpad8;                                  // The numpad 8 key
        case XK_KP_Page_Up:
            return Keyboard::Key::Numpad9;                                  // The numpad 9 key

        case XK_BackSpace:
            return Keyboard::Key::BackSpace;                                // The Backspace key
        case XK_Tab:
            return Keyboard::Key::Tab;                                      // The Tabulation key
        case XK_Prior:
            return Keyboard::Key::PageUp;                                   // The Page up key
        case XK_Next:
            return Keyboard::Key::PageDown;                                 // The Page down key
        case XK_End:
            return Keyboard::Key::End;                                      // The End key
        case XK_Home:
            return Keyboard::Key::Home;                                     // The Home key
        case XK_Insert:
            return Keyboard::Key::Insert;                                   // The Insert key
        case XK_Delete:
            return Keyboard::Key::Delete;                                   // The Delete key

        case XK_Left:
            return Keyboard::Key::Left;                                     // Left arrow
        case XK_Right:
            return Keyboard::Key::Right;                                    // Right arrow
        case XK_Up:
            return Keyboard::Key::Up;                                       // Up arrow
        case XK_Down:
            return Keyboard::Key::Down;                                     // Down arrow

        case XK_Pause:
            return Keyboard::Key::Pause;                                    // The Pause key
        case XK_Caps_Lock:
            return Keyboard::Key::CapsLock;                                 // The Caps Lock key
        default:
            return Keyboard::Key::Unknown;
    }
}

static Bool selectEvents(Display*, XEvent* event, XPointer) {
    return (event->type == ClientMessage ||
            event->type == DestroyNotify ||
            event->type == KeyPress ||
            event->type == KeyRelease ||
            event->type == ButtonPress ||
            event->type == ButtonRelease ||
            event->type == MotionNotify ||
            event->type == ConfigureNotify ||
            event->type == LeaveNotify ||
            event->type == EnterNotify);
}

/**
 * @brief      This functions filters event depending on the value of _keyRepeat.
 *
 * @param      xEvent  The x event to filter.
 *
 * @return     True if the event should be ignored, False if it should be keeped.
 */
bool WindowImpl::shouldIgnoreRepeated(XEvent& xEvent) {
    // (code shamelessly taken from the SFML, which took it from SDL) ;)

    // We are only interested in filtering KeyRelease events, i.e. from now on we only
    // have KeyRelease events
    if (xEvent.type != KeyRelease) {
        return false;
    }

    // Check if there's a matching KeyPress event in the queue, else we can return
    if (XPending(_display) == 0) {
        return false;
    }

    // Grab it but don't remove it from the queue, it still needs to be processed
    XEvent nextEvent;
    XPeekEvent(_display, &nextEvent);

    // Again, we're only interested in a corresponding KeyPress
    if (nextEvent.type != KeyPress) {
        return false;
    }

    // Check if it is a duplicated event (same timestamp as the KeyRelease event)
    if ((nextEvent.xkey.keycode == xEvent.xkey.keycode) &&
        (nextEvent.xkey.time - xEvent.xkey.time < 2)) {
        // If we don't want repeated events, remove the next KeyPress from the queue
        if (!_keyRepeat) {
            XNextEvent(_display, &nextEvent);
        }

        // This KeyRelease is a repeated event and we don't want it
        return true;
    }

    return false;
}

bool WindowImpl::pollEvent(Event& event) {
    XEvent xEvent;

    if (XCheckIfEvent(_display, &xEvent, selectEvents, nullptr) == False) {
        return false;
    }

    if (shouldIgnoreRepeated(xEvent)) {
        return false;
    }

    switch (xEvent.type) {
        case ClientMessage:
            if (xEvent.xclient.message_type == _wmProtocols && static_cast<Atom>(xEvent.xclient.data.l[0]) == _wmDeleteWindow) {
                event.type = Event::Type::Close;
            } else {
                return false;
            }

            break;

        case DestroyNotify:
            event.type = Event::Type::Close;
            break;

        case KeyPress:
            event.type          = Event::Type::KeyPressed;
            event.key.code      = keysymToLugKey(XLookupKeysym((&xEvent.xkey), 0)); // TODO: indexes?
            event.key.ctrl      = (xEvent.xkey.state & ControlMask) != 0;
            event.key.alt       = (xEvent.xkey.state & Mod1Mask) != 0;
            event.key.shift     = (xEvent.xkey.state & ShiftMask) != 0;
            event.key.system    = (xEvent.xkey.state & Mod4Mask) != 0;
            break;

        case KeyRelease:
            event.type          = Event::Type::KeyReleased;
            event.key.code      = keysymToLugKey(XLookupKeysym((&xEvent.xkey), 0)); // TODO: indexes?
            event.key.ctrl      = (xEvent.xkey.state & ControlMask) != 0;
            event.key.alt       = (xEvent.xkey.state & Mod1Mask) != 0;
            event.key.shift     = (xEvent.xkey.state & ShiftMask) != 0;
            event.key.system    = (xEvent.xkey.state & Mod4Mask) != 0;
            break;

        case ConfigureNotify:
            if (xEvent.xconfigure.width != _parent->_mode.width || xEvent.xconfigure.height != _parent->_mode.height) {
                _parent->_mode.width = xEvent.xconfigure.width;
                _parent->_mode.height = xEvent.xconfigure.height;

                event.type = Event::Type::Resize;
            } else {
                return false;
            }

        case ButtonPress:
            switch (xEvent.xbutton.button) {
                case Button4:
                    event.type = Event::Type::MouseWheel;
                    event.mouse.scrollOffset.xOffset = 0;
                    event.mouse.scrollOffset.yOffset = 1;
                    break;
                case Button5:
                    event.type = Event::Type::MouseWheel;
                    event.mouse.scrollOffset.xOffset = 0;
                    event.mouse.scrollOffset.yOffset = -1;
                    break;
                case 6:
                    event.type = Event::Type::MouseWheel;
                    event.mouse.scrollOffset.xOffset = 1;
                    event.mouse.scrollOffset.yOffset = 0;
                    break;
                case 7:
                    event.type = Event::Type::MouseWheel;
                    event.mouse.scrollOffset.xOffset = -1;
                    event.mouse.scrollOffset.yOffset = 0;
                    break;
                default:
                    event.type = Event::Type::ButtonPressed;
                    event.mouse.code = buttonCodeToLugButton(xEvent.xbutton.button);
            }
            event.mouse.coord.x = xEvent.xbutton.x;
            event.mouse.coord.y = xEvent.xbutton.y;
            event.mouse.ctrl = (xEvent.xbutton.state & ControlMask) != 0;
            event.mouse.shift = (xEvent.xbutton.state & ShiftMask) != 0;
            break;

        case ButtonRelease:
            if (xEvent.xbutton.button != Button4 && xEvent.xbutton.button != Button5 &&
                xEvent.xbutton.button != 6 && xEvent.xbutton.button != 7) {
                event.type = Event::Type::ButtonReleased;
                event.mouse.code = buttonCodeToLugButton(xEvent.xbutton.button);
                event.mouse.coord.x = xEvent.xbutton.x;
                event.mouse.coord.y = xEvent.xbutton.y;
            } else {
                return false;
            }
            break;

        case MotionNotify:
            event.type = Event::Type::MouseMoved;
            event.mouse.coord.x = xEvent.xbutton.x;
            event.mouse.coord.y = xEvent.xbutton.y;
            break;

        case LeaveNotify:
            event.type = Event::Type::MouseLeave;
            event.mouse.coord.x = xEvent.xbutton.x;
            event.mouse.coord.y = xEvent.xbutton.y;
            break;

        case EnterNotify:
            event.type = Event::Type::MouseEnter;
            event.mouse.coord.x = xEvent.xbutton.x;
            event.mouse.coord.y = xEvent.xbutton.y;
            break;

        default:
            return false;
    }

    return true;
}

Display* WindowImpl::getDisplay() const {
    return _display;
}

::Window WindowImpl::getWindow() const {
    return _window;
}

void WindowImpl::setKeyRepeat(bool state) {
    _keyRepeat = state;
}

void WindowImpl::setWindowDecorations(Style style) {
    WMHints hints;

    std::memset(&hints, 0, sizeof(hints));
    hints.flags = MWMHintsFunctions | MWMHintsDecorations;

    if (static_cast<bool>(style & Style::Titlebar)) {
        hints.decorations |= MWMDecorBorder | MWMDecorTitle | MWMDecorMinimize | MWMDecorMenu;
        hints.functions |= MWMFuncMove | MWMFuncMinimize;
    }

    if (static_cast<bool>(style & Style::Resize)) {
        hints.decorations |= MWMDecorMaximize | MWMDecorResizeh;
        hints.functions |= MWMFuncMaximize | MWMFuncResize;
    }

    if (static_cast<bool>(style & Style::Close)) {
        hints.decorations |= 0;
        hints.functions |= MWMFuncClose;
    }

    XChangeProperty(
        _display,
        _window,
        _wmHints,
        _wmHints,
        32, // 32 bits
        PropModeReplace, // Replace existing
        reinterpret_cast<const unsigned char*>(&hints),
        5); // 5 elements
}

} // priv
} // Window
} // lug
