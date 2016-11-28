#include <lug/Config.hpp>
#include <lug/Window/Window.hpp>
#include <string>

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "vulkanExample", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "vulkanExample", __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "vulkanExample", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "vulkanExample", __VA_ARGS__))

int main() {
    auto window = lug::Window::Window::create(800, 600, std::string("Default Window"), lug::Window::Style::Default);
    
    if (!window) {
        return 1;
    }

    // While window is open execute the following
    while (window->isOpen()) {
        lug::Window::Event event;
        while (window->pollEvent(event)) {
            LOGI("%d\n", event.type);
            if (event.type == lug::Window::EventType::CLOSE) {
                window->close();
            }

        }

        // ...
    }
    
    return 0;
}
