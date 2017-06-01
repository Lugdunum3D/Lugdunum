#include <cstdlib>
#include <lug/Window/Win32/WindowImplWin32.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Window {
namespace priv {

uint8_t WindowImpl::windowCount = 0;
WindowImpl* WindowImpl::fullscreenWindow = nullptr;

WindowImpl::WindowImpl(Window* win) : _parent{win} {}

WindowImpl::~WindowImpl() {
    // Destroy the custom icon, if any
    if (_icon) {
        DestroyIcon(_icon);
    }

    if (!_callback) {
        // Destroy the window
        if (_handle) {
            DestroyWindow(_handle);
        }

        // Decrement the window count
        windowCount--;

        // Unregister window class if we were the last window
        if (windowCount == 0) {
            UnregisterClassW(className, GetModuleHandleW(NULL));
        }
    } else {
        // The window is external: remove the hook on its message callback
        SetWindowLongPtrW(_handle, GWLP_WNDPROC, _callback);
    }
}

bool WindowImpl::init(const Window::InitInfo& initInfo) {

    // Register the window class at first call
    if (windowCount == 0) {
        _hinstance = GetModuleHandleW(nullptr);
        registerWindow();
    }

    // Compute position and size
    HDC screenDC = GetDC(nullptr);
    const uint16_t left = static_cast<uint16_t>(GetDeviceCaps(screenDC, HORZRES) - initInfo.width) / 2;
    const uint16_t top = static_cast<uint16_t>(GetDeviceCaps(screenDC, VERTRES) - initInfo.height) / 2;
    ReleaseDC(nullptr, screenDC);

    // Choose the window style according to the Style parameter
    DWORD win32Style = WS_VISIBLE;

    if (initInfo.style == Style::None) {
        win32Style |= WS_POPUP;
    } else {
        if ((initInfo.style & Style::Titlebar) == Style::Titlebar) {
            win32Style |= WS_CAPTION | WS_MINIMIZEBOX;
        }

        if ((initInfo.style & Style::Resize) == Style::Resize) {
            win32Style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        }

        if ((initInfo.style & Style::Close) == Style::Close) {
            win32Style |= WS_SYSMENU;
        }
    }

    _fullscreen = (initInfo.style & Style::Fullscreen) == Style::Fullscreen ? true : false;
    // In windowed mode, adjust width and height so that the window will have the requested client area
    RECT rectangle = { 0, 0, initInfo.width, initInfo.height };

    if (!_fullscreen) {
        AdjustWindowRect(&rectangle, win32Style, false);
    }

#pragma warning(push)
#pragma warning(disable : 4996)
    std::vector<wchar_t> realTitle(initInfo.title.length() + 1); // Convert a const char * to a wchar_t
    mbstowcs(&realTitle[0], initInfo.title.c_str(), initInfo.title.length() + 1);
#pragma warning(pop)

    _handle = CreateWindowW(
        className,
        &realTitle[0],
        win32Style,
        left,
        top,
        rectangle.right - rectangle.left,
        rectangle.bottom - rectangle.top,
        nullptr,
        nullptr,
        GetModuleHandleW(nullptr),
        this);

    // Switch to fullscreen if requested
    if (_fullscreen) {
        if (activateFullscreen()) {
            // TODO: Log failure
        }
    }

    // Increment window count
    windowCount++;
    return true;
}

void WindowImpl::close() {
    // Restore the previous video mode (in case we were running in fullscreen)
    if (fullscreenWindow == this) {
        ChangeDisplaySettingsW(nullptr, 0);
        fullscreenWindow = nullptr;
    }

    // No longer capture the cursor
    ReleaseCapture();
}

static Keyboard::Key vkToLugKey(WPARAM key) {
    switch (key) {
        case VK_F1:
            return Keyboard::Key::F1;                           // The F1 key
        case VK_F2:
            return Keyboard::Key::F2;                           // The F2 key
        case VK_F3:
            return Keyboard::Key::F3;                           // The F3 key
        case VK_F4:
            return Keyboard::Key::F4;                           // The F4 key
        case VK_F5:
            return Keyboard::Key::F5;                           // The F5 key
        case VK_F6:
            return Keyboard::Key::F6;                           // The F6 key
        case VK_F7:
            return Keyboard::Key::F7;                           // The F7 key
        case VK_F8:
            return Keyboard::Key::F8;                           // The F8 key
        case VK_F9:
            return Keyboard::Key::F9;                           // The F9 key
        case VK_F10:
            return Keyboard::Key::F10;                          // The F10 key
        case VK_F11:
            return Keyboard::Key::F11;                          // The F11 key
        case VK_F12:
            return Keyboard::Key::F12;                          // The F12 key
        case VK_F13:
            return Keyboard::Key::F13;                          // The F13 key
        case VK_F14:
            return Keyboard::Key::F14;                          // The F14 key
        case VK_F15:
            return Keyboard::Key::F15;                          // The F15 key

        case 'A':
            return Keyboard::Key::A;                            // The A key
        case 'B':
            return Keyboard::Key::B;                            // The B key
        case 'C':
            return Keyboard::Key::C;                            // The C key
        case 'D':
            return Keyboard::Key::D;                            // The D key
        case 'E':
            return Keyboard::Key::E;                            // The E key
        case 'F':
            return Keyboard::Key::F;                            // The F key
        case 'G':
            return Keyboard::Key::G;                            // The G key
        case 'H':
            return Keyboard::Key::H;                            // The H key
        case 'I':
            return Keyboard::Key::I;                            // The I key
        case 'J':
            return Keyboard::Key::J;                            // The J key
        case 'K':
            return Keyboard::Key::K;                            // The K key
        case 'L':
            return Keyboard::Key::L;                            // The L key
        case 'M':
            return Keyboard::Key::M;                            // The M key
        case 'N':
            return Keyboard::Key::N;                            // The N key
        case 'O':
            return Keyboard::Key::O;                            // The O key
        case 'P':
            return Keyboard::Key::P;                            // The P key
        case 'Q':
            return Keyboard::Key::Q;                            // The Q key
        case 'R':
            return Keyboard::Key::R;                            // The R key
        case 'S':
            return Keyboard::Key::S;                            // The S key
        case 'T':
            return Keyboard::Key::T;                            // The T key
        case 'U':
            return Keyboard::Key::U;                            // The U key
        case 'V':
            return Keyboard::Key::V;                            // The V key
        case 'W':
            return Keyboard::Key::W;                            // The W key
        case 'X':
            return Keyboard::Key::X;                            // The X key
        case 'Y':
            return Keyboard::Key::Y;                            // The Y key
        case 'Z':
            return Keyboard::Key::Z;                            // The Z key

        case '0':
            return Keyboard::Key::Num0;                         // The 0 key
        case '1':
            return Keyboard::Key::Num1;                         // The 1 key
        case '2':
            return Keyboard::Key::Num2;                         // The 2 key
        case '3':
            return Keyboard::Key::Num3;                         // The 3 key
        case '4':
            return Keyboard::Key::Num4;                         // The 4 key
        case '5':
            return Keyboard::Key::Num5;                         // The 5 key
        case '6':
            return Keyboard::Key::Num6;                         // The 6 key
        case '7':
            return Keyboard::Key::Num7;                         // The 7 key
        case '8':
            return Keyboard::Key::Num8;                         // The 8 key
        case '9':
            return Keyboard::Key::Num9;                         // The 9 key

        case VK_ESCAPE:
            return Keyboard::Key::Escape;                       // The Escape key

        case VK_LCONTROL:
            return Keyboard::Key::LControl;                     // The left Control key
        case VK_LSHIFT:
            return Keyboard::Key::LShift;                       // The left Shift key
        case VK_LMENU:
            return Keyboard::Key::LAlt;                         // The left Alt key
        case VK_LWIN:
            return Keyboard::Key::LSystem;                      // The left OS specific key: window (Windows and Linux), apple (MacOS X), ...

        case VK_RCONTROL:
            return Keyboard::Key::RControl;                     // The right Control key
        case VK_RSHIFT:
            return Keyboard::Key::RShift;                       // The right Shift key
        case VK_RMENU:
            return Keyboard::Key::RAlt;                         // The right Alt key
        case VK_RWIN:
            return Keyboard::Key::RSystem;                      // The right OS specific key: window (Windows and Linux), apple (MacOS X), ...

        case VK_APPS:
            return Keyboard::Key::Menu;                         // The Menu key
        case VK_OEM_4:
            return Keyboard::Key::LBracket;                     // The [ key
        case VK_OEM_6:
            return Keyboard::Key::RBracket;                     // The ] key
        case VK_OEM_1:
            return Keyboard::Key::SemiColon;                    // The , key
        case VK_OEM_COMMA:
            return Keyboard::Key::Comma;                        // The , key
        case VK_OEM_PERIOD:
            return Keyboard::Key::Period;                       // The . key
        case VK_OEM_7:
            return Keyboard::Key::Quote;                        // The ' key
        case VK_OEM_2:
            return Keyboard::Key::Slash;                        // The / key
        case VK_OEM_5:
            return Keyboard::Key::BackSlash;                    // The \ key
        case VK_OEM_3:
            return Keyboard::Key::Tilde;                        // The ~ key
        case VK_OEM_PLUS:
            return Keyboard::Key::Equal;                        // The = key
        case VK_OEM_MINUS:
            return Keyboard::Key::Dash;                         // The - key
        case VK_SPACE:
            return Keyboard::Key::Space;                        // The Space key
        case VK_RETURN:
            return Keyboard::Key::Return;                       // The Return key

        case VK_ADD:
            return Keyboard::Key::Add;                          // The + key
        case VK_SUBTRACT:
            return Keyboard::Key::Subtract;                     // The - key
        case VK_MULTIPLY:
            return Keyboard::Key::Multiply;                     // The * key
        case VK_DIVIDE:
            return Keyboard::Key::Divide;                       // The / key

        case VK_NUMPAD0:
            return Keyboard::Key::Numpad0;                      // The numpad 0 key
        case VK_NUMPAD1:
            return Keyboard::Key::Numpad1;                      // The numpad 1 key
        case VK_NUMPAD2:
            return Keyboard::Key::Numpad2;                      // The numpad 2 key
        case VK_NUMPAD3:
            return Keyboard::Key::Numpad3;                      // The numpad 3 key
        case VK_NUMPAD4:
            return Keyboard::Key::Numpad4;                      // The numpad 4 key
        case VK_NUMPAD5:
            return Keyboard::Key::Numpad5;                      // The numpad 5 key
        case VK_NUMPAD6:
            return Keyboard::Key::Numpad6;                      // The numpad 6 key
        case VK_NUMPAD7:
            return Keyboard::Key::Numpad7;                      // The numpad 7 key
        case VK_NUMPAD8:
            return Keyboard::Key::Numpad8;                      // The numpad 8 key
        case VK_NUMPAD9:
            return Keyboard::Key::Numpad9;                      // The numpad 9 key

        case VK_BACK:
            return Keyboard::Key::BackSpace;                    // The Backspace key
        case VK_TAB:
            return Keyboard::Key::Tab;                          // The Tabulation key
        case VK_PRIOR:
            return Keyboard::Key::PageUp;                       // The Page up key
        case VK_NEXT:
            return Keyboard::Key::PageDown;                     // The Page down key
        case VK_END:
            return Keyboard::Key::End;                          // The End key
        case VK_HOME:
            return Keyboard::Key::Home;                         // The Home key
        case VK_INSERT:
            return Keyboard::Key::Insert;                       // The Insert key
        case VK_DELETE:
            return Keyboard::Key::Delete;                       // The Delete key

        case VK_LEFT:
            return Keyboard::Key::Left;                         // Left arrow
        case VK_RIGHT:
            return Keyboard::Key::Right;                        // Right arrow
        case VK_UP:
            return Keyboard::Key::Up;                           // Up arrow
        case VK_DOWN:
            return Keyboard::Key::Down;                         // Down arrow

        case VK_PAUSE:
            return Keyboard::Key::Pause;                        // The Pause key
        default:
            return Keyboard::Key::Unknown;
    }
}

bool WindowImpl::pollEvent(Event& event) {
    // We process the window events only if we own it
    if (!_callback) {
        MSG message;

        while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }

    if (!_events.empty()) {
        event = _events.front();
        _events.pop();
        return true;
    }

    return false;
}

void WindowImpl::setKeyRepeat(bool state) {
    _keyRepeat = state;
}

HWND WindowImpl::getHandle() const {
    return _handle;
}

HINSTANCE WindowImpl::getHinstance() const {
    return _hinstance;
}

void WindowImpl::processWindowEvents(UINT message, WPARAM wParam, LPARAM lParam) {
    Event e;

    switch (message) {
        case WM_SIZE:
            e.type = Event::Type::Resize;

            // The handle is not set on first call
            if (!_handle) {
                break;
            }

            RECT rect;

            if (GetClientRect(_handle, &rect)) {
                _parent->_mode.width = static_cast<uint16_t>(rect.right - rect.left);
                _parent->_mode.height = static_cast<uint16_t>(rect.bottom - rect.top);

                if (!_parent->_mode.width || !_parent->_mode.height) {
                    return;
                }
            } else {
                LUG_LOG.error("WindowImpl::Win32::processWindowEvents Can't get window rect");
            }
            break;

        case WM_CLOSE:
            e.type = Event::Type::Close;
            break;

        case WM_DESTROY:
            e.type = Event::Type::Destroy;
            break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (_keyRepeat || ((HIWORD(lParam) & KF_REPEAT) == 0)) {
                e.type = Event::Type::KeyPressed;
                configKeyEvent(e.key, wParam, lParam);
            } else {
                return;
            }
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            e.type = Event::Type::KeyReleased;
            configKeyEvent(e.key, wParam, lParam);
            break;

        case WM_CHAR:
            if (_keyRepeat || ((lParam & (1 << 30)) == 0)) {
                e.type = Event::Type::CharEntered;
                e.character.val = static_cast<wchar_t>(wParam);
            } else {
                return;
            }
            break;

        case WM_RBUTTONDOWN:
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_XBUTTONDOWN:
            e.type = Event::Type::ButtonPressed;
            configMouseEvent(e.mouse, wParam, lParam);
            configMouseButtonEvent(e.mouse, message, wParam, lParam);
            break;

        case WM_RBUTTONUP:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP:
            e.type = Event::Type::ButtonReleased;
            configMouseEvent(e.mouse, wParam, lParam);
            configMouseButtonEvent(e.mouse, message, wParam, lParam);
           break;

        case WM_MOUSEMOVE:
            e.type = Event::Type::MouseMoved;
            e.mouse.code = Mouse::Button::Unknown;
            configMouseEvent(e.mouse, wParam, lParam);
            break;

        case WM_MOUSEWHEEL:
        case WM_MOUSEHWHEEL:
            e.type = Event::Type::MouseWheel;
            configMouseEvent(e.mouse, wParam, lParam);
            configMouseWheelEvent(e.mouse, message, wParam);
            break;

        default:
            return;
    }

    _events.push(std::move(e));
}

void WindowImpl::registerWindow() const {
    // Register the window class with windows
    WNDCLASSW windowClass = {};

    windowClass.style = 0;
    windowClass.lpfnWndProc = &WindowImpl::onEvent;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = _hinstance;
    windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = 0;
    windowClass.lpszMenuName = nullptr;
    windowClass.lpszClassName = className;

    if (!RegisterClassW(&windowClass)) {
        MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
    }
}

bool WindowImpl::activateFullscreen() {
    DEVMODEW devMode{};

    devMode.dmSize = sizeof(devMode);               // Size Of The Devmode Structure
    devMode.dmPelsWidth = _parent->_mode.width;     // Selected Screen Width
    devMode.dmPelsHeight = _parent->_mode.height;   // Selected Screen Height
    devMode.dmBitsPerPel = 32;                      // Selected Bits Per Pixel
    devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    // Apply fullscreen mode
    if (ChangeDisplaySettingsW(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
        // If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
        if (MessageBox(NULL, "Fullscreen mode has failed to be initialized. Use Windowed Mode Instead?", "Warning", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
            _fullscreen = false;
            return false;
        } else {
            // Pop Up A Message Box Letting User Know The Program Is Closing.
            MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
            return false;
        }
    }

    // Make the window flags compatible with fullscreen mode
    SetWindowLongW(_handle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
    SetWindowLongW(_handle, GWL_EXSTYLE, WS_EX_APPWINDOW);

    // Resize the window so that it fits the entire screen
    SetWindowPos(_handle, HWND_TOP, 0, 0, _parent->_mode.width, _parent->_mode.height, SWP_FRAMECHANGED);
    ShowWindow(_handle, SW_SHOW);

    // Set "this" as the current fullscreen window
    fullscreenWindow = this;
    return true;
}

Keyboard::Key WindowImpl::getKeyCode(WPARAM wParam, LPARAM lParam) {
    WPARAM new_vk = wParam;
    UINT scancode = (lParam & 0x00ff0000) >> 16;
    int extended = (lParam & 0x01000000) != 0;

    switch (wParam) {
        case VK_SHIFT:
            new_vk = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
            break;
        case VK_CONTROL:
            new_vk = extended ? VK_RCONTROL : VK_LCONTROL;
            break;
        case VK_MENU:
            new_vk = extended ? VK_RMENU : VK_LMENU;
            break;
        default:
            // Not a key we map from generic to left/right specialized, just return it.
            break;
    }

    return vkToLugKey(new_vk);
}

void WindowImpl::configKeyEvent(KeyEvent& key, WPARAM wParam, LPARAM lParam) {
    key.code = getKeyCode(wParam, lParam);
    key.alt = HIWORD(GetAsyncKeyState(VK_MENU)) != 0;
    key.ctrl = HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0;
    key.shift = HIWORD(GetAsyncKeyState(VK_SHIFT)) != 0;
    key.system = HIWORD(GetAsyncKeyState(VK_LWIN)) || HIWORD(GetAsyncKeyState(VK_RWIN));
}

void WindowImpl::configMouseButtonEvent(MouseEvent& key, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_RBUTTONUP:
        case WM_RBUTTONDOWN:
            key.code = Mouse::Button::Right;
            break;
        case WM_LBUTTONUP:
        case WM_LBUTTONDOWN:
            key.code = Mouse::Button::Left;
            break;
        case WM_MBUTTONUP:
        case WM_MBUTTONDOWN:
            key.code = Mouse::Button::Middle;
            break;
        case WM_XBUTTONUP:
        case WM_XBUTTONDOWN:
            key.code = HIWORD(wParam) == XBUTTON1 ? Mouse::Button::XButton1 : Mouse::Button::XButton2;
            break;
        default:
            key.code = Mouse::Button::Unknown;
            break;
    }

    key.coord.x = static_cast<int32_t>(LOWORD(lParam));
    key.coord.y = static_cast<int32_t>(HIWORD(lParam));
}

void WindowImpl::configMouseEvent(MouseEvent & mouse, WPARAM wParam, LPARAM lParam) {
    mouse.ctrl = false;
    mouse.shift = false;
    mouse.left = false;
    mouse.middle = false;
    mouse.right = false;
    mouse.x1 = false;
    mouse.x2 = false;

    getMouseCoord(mouse, lParam);
    getMouseEventModifier(mouse, wParam);
}

void WindowImpl::getMouseEventModifier(MouseEvent & mouse, WPARAM wParam) {
    int keyModifier = GET_KEYSTATE_WPARAM(wParam);

    if (keyModifier & MK_CONTROL) {
        mouse.ctrl = true;
    }

    if (keyModifier & MK_SHIFT) {
        mouse.shift = true;
    }

    if (keyModifier & MK_LBUTTON) {
        mouse.left = true;
    }

    if (keyModifier & MK_MBUTTON) {
        mouse.middle = true;
    }

    if (keyModifier & MK_RBUTTON) {
        mouse.right = true;
    }

    if (keyModifier & MK_XBUTTON1) {
        mouse.x1 = true;
    }

    if (keyModifier & MK_XBUTTON2) {
        mouse.x2 = true;
    }
}

void WindowImpl::getMouseCoord(MouseEvent & mouse, LPARAM lParam) {
    mouse.coord.x = static_cast<int32_t>(LOWORD(lParam));
    mouse.coord.y = static_cast<int32_t>(HIWORD(lParam));
}

void WindowImpl::configMouseWheelEvent(MouseEvent & mouse, UINT message, WPARAM wParam)
{
    double xOffset = 0.0;
    double yOffset = 0.0;

    if (message == WM_MOUSEWHEEL) {
        xOffset = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
    } else if (message == WM_MOUSEHWHEEL) {
        yOffset = -(GET_WHEEL_DELTA_WPARAM(wParam)) / WHEEL_DELTA;
    }
    mouse.scrollOffset.xOffset = xOffset;
    mouse.scrollOffset.yOffset = yOffset;
}

LRESULT CALLBACK WindowImpl::onEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
    // Associate handle and Window instance when the creation message is received
    if (message == WM_CREATE) {
        // Get WindowImplWin32 instance (it was passed as the last argument of CreateWindow)
        LONG_PTR window = (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams;

        // Set as the "user data" parameter of the window
        SetWindowLongPtrW(handle, GWLP_USERDATA, window);
    }

    // Get the WindowImpl instance corresponding to the window handle
    WindowImpl* window = handle ? reinterpret_cast<WindowImpl*>(GetWindowLongPtr(handle, GWLP_USERDATA)) : nullptr;

    // Forward the event to the appropriate function
    if (window) {
        window->processWindowEvents(message, wParam, lParam);

        if (window->_callback) {
            return CallWindowProcW(reinterpret_cast<WNDPROC>(window->_callback), handle, message, wParam, lParam);
        }
    }

    // We don't forward the WM_CLOSE message to prevent the OS from automatically destroying the window
    if (message == WM_CLOSE) {
        return 0;
    }

    // Don't forward the menu system command, so that pressing ALT or F10 doesn't steal the focus
    if ((message == WM_SYSCOMMAND) && (wParam == SC_KEYMENU)) {
        return 0;
    }

    return DefWindowProcW(handle, message, wParam, lParam);
}

} // priv
} // Window
} // lug
