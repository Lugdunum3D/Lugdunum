#include <lug/Config.hpp>
#include <lug/Window/Window.hpp>
#include <string>

#if defined(__ANDROID__)
    #include <android/log.h>
    #define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "lugdunum", __VA_ARGS__))
#endif
int main() {
    auto window = lug::Window::Window::create(800, 600, "Default Window", lug::Window::Style::Default);
    
    if (!window) {
        return 1;
    }

    // While window is open execute the following
    while (window->isOpen()) {
        lug::Window::Event event;
        while (window->pollEvent(event)) {
            #if defined(__ANDROID__)
                LOGI("%d\n", event.type);
            #endif
            if (event.type == lug::Window::EventType::CLOSE) {
                window->close();
            }

        }

        // ...
    }
    
    return 0;
}
