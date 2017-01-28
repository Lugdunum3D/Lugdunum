#include <cstdlib>
#include <lug/Window/Win32/WindowImplWin32.hpp>

uint8_t lug::Window::priv::WindowImpl::windowCount = 0;
lug::Window::priv::WindowImpl* lug::Window::priv::WindowImpl::fullscreenWindow = nullptr;

lug::Window::priv::WindowImpl::WindowImpl(Window* win) : _parent{win} {}

lug::Window::priv::WindowImpl::~WindowImpl() {
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
    }
    else {
        // The window is external: remove the hook on its message callback
        SetWindowLongPtrW(_handle, GWLP_WNDPROC, _callback);
    }
}

bool lug::Window::priv::WindowImpl::create(const std::string& title, Style style) {
    
    // Register the window class at first call
    if (windowCount == 0) {
        registerWindow();
    }

    // Compute position and size
    HDC screenDC = GetDC(nullptr);
    const uint16_t left = static_cast<uint16_t>(GetDeviceCaps(screenDC, HORZRES) - _parent->_mode.width) / 2;
    const uint16_t top = static_cast<uint16_t>(GetDeviceCaps(screenDC, VERTRES) - _parent->_mode.height) / 2;
    ReleaseDC(nullptr, screenDC);

    // Choose the window style according to the Style parameter
    DWORD win32Style = WS_VISIBLE;
    if (style == Style::None) {
        win32Style |= WS_POPUP;
    } else {
        if ((style & Style::Titlebar) == Style::Titlebar) {
            win32Style |= WS_CAPTION | WS_MINIMIZEBOX;
        }

        if ((style & Style::Resize) == Style::Resize) {
            win32Style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        }

        if ((style & Style::Close) == Style::Close) {
            win32Style |= WS_SYSMENU;
        }
    }

    _fullscreen = (style & Style::Fullscreen) == Style::Fullscreen ? true : false;
    // In windowed mode, adjust width and height so that window will have the requested client area
    RECT rectangle = { 0, 0, _parent->_mode.width, _parent->_mode.height };
    if (!_fullscreen) {
        AdjustWindowRect(&rectangle, win32Style, false);
    }

#pragma warning ( push )
#pragma warning (disable : 4996 )
    std::vector<wchar_t> realTitle(title.length() + 1); // Convert a const char * to a wchar_t
    mbstowcs(&realTitle[0], title.c_str(), title.length() + 1);
#pragma warning ( pop )

    _handle = CreateWindowW(className, &realTitle[0], win32Style, left, top, rectangle.right - rectangle.left, rectangle.bottom - rectangle.top, nullptr, nullptr, GetModuleHandleW(nullptr), this);

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

void lug::Window::priv::WindowImpl::close() {
    // Restore the previous video mode (in case we were running in fullscreen)
    if (fullscreenWindow == this) {
        ChangeDisplaySettingsW(nullptr, 0);
        fullscreenWindow = nullptr;
    }

    // No longer capture the cursor
    ReleaseCapture();
}

bool lug::Window::priv::WindowImpl::pollEvent(lug::Window::Event& event) {
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

void lug::Window::priv::WindowImpl::processWindowEvents(UINT message, WPARAM wParam, LPARAM lParam) {
    Event e;

    switch (message) {
    case WM_CLOSE:
        e.type = EventType::CLOSE;
        break;

    case WM_DESTROY:
        e.type = EventType::DESTROY;
        break;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        e.type = EventType::KEY_DOWN;
        configKeyEvent(e.key, wParam, lParam);
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        e.type = EventType::KEY_UP;
        configKeyEvent(e.key, wParam, lParam);
        break;

    default:
        return;
    }

    _events.push(std::move(e));
}

void lug::Window::priv::WindowImpl::registerWindow() const {
    // Register the window class with windows
    WNDCLASSW windowClass = {};
    windowClass.style = 0;
    windowClass.lpfnWndProc = &lug::Window::priv::WindowImpl::onEvent;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = GetModuleHandleW(nullptr);
    windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = 0;
    windowClass.lpszMenuName = nullptr;
    windowClass.lpszClassName = className;

    if (!RegisterClassW(&windowClass)) {
        MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
    }
}

bool lug::Window::priv::WindowImpl::activateFullscreen() {
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

lug::Window::Keyboard::Key lug::Window::priv::WindowImpl::getKeyCode(WPARAM wParam, LPARAM lParam) {
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
        // not a key we map from generic to left/right specialized
        //  just return it.
        break;
    }

    return static_cast<Keyboard::Key>(new_vk);
}

void lug::Window::priv::WindowImpl::configKeyEvent(KeyEvent& key, WPARAM wParam, LPARAM lParam) {
    key.code = getKeyCode(wParam, lParam);
    key.alt = HIWORD(GetAsyncKeyState(VK_MENU)) != 0;
    key.ctrl = HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0;
    key.shift = HIWORD(GetAsyncKeyState(VK_SHIFT)) != 0;
    key.system = HIWORD(GetAsyncKeyState(VK_LWIN)) || HIWORD(GetAsyncKeyState(VK_RWIN));
}

////////////////////////////////////////////////////////////
LRESULT CALLBACK lug::Window::priv::WindowImpl::onEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
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
