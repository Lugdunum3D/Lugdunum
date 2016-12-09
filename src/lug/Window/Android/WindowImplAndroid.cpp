#include <lug/Window/Android/WindowImplAndroid.hpp>

/* todo : to remove once we have the logger */
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "lugdunum", __VA_ARGS__))


namespace lug {
namespace Window {
namespace priv {

lug::Window::priv::WindowImpl::WindowImpl(Window*) {}

bool lug::Window::priv::WindowImpl::create(const std::string&, Style) {
    return true;
}

void lug::Window::priv::WindowImpl::close() {}

bool lug::Window::priv::WindowImpl::pollEvent(lug::Window::Event& event) {

    if (lug::Main::inputQueue != nullptr) {
        AInputEvent *androidEvent = nullptr;
        while (AInputQueue_getEvent(lug::Main::inputQueue, &androidEvent) >= 0) {
            LOGI("New input event: type=%d\n", AInputEvent_getType(androidEvent));

            lug::Window::Event e;
            e.type = lug::Window::EventType::INPUT;
            lug::Main::events.push(std::move(e));

            if (AInputQueue_preDispatchEvent(lug::Main::inputQueue, androidEvent)) {
                continue;
            }
            int32_t handled = 0;
            AInputQueue_finishEvent(lug::Main::inputQueue, androidEvent, handled);
        }
    }

    if (!lug::Main::events.empty()) {
        event = lug::Main::events.front();
        lug::Main::events.pop();
        return true;
    }

    return false;
}

} // namespace priv
} // namespace Window
} // namespace lug
