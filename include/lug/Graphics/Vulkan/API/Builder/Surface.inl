#if defined(LUG_SYSTEM_WINDOWS) // Win32 surface
inline void Surface::setWindowInformations(HINSTANCE hinstance, HWND hwnd) {
    _hinstance = hinstance;
    _hwnd = hwnd;
}
#elif defined(LUG_SYSTEM_LINUX) // Linux surface
inline void Surface::setWindowInformations(Display* dpy, ::Window window) {
    _dpy = dpy;
    _window = window;
}
#elif defined(LUG_SYSTEM_ANDROID) // Android Surface
inline void Surface::setWindowInformations(ANativeWindow* window) {
    _window = window;
}
#endif
