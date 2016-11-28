#include <lug/Window/Android/WindowImplAndroid.hpp>
#include <android/input.h>

/* todo : to remove once we have the logger */
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "lugdunum", __VA_ARGS__))

struct androidApp androidEventQueue;

lug::Window::priv::WindowImpl::WindowImpl(Window*) {}

bool lug::Window::priv::WindowImpl::create(const std::string&, Style) {
    return true;
}

void lug::Window::priv::WindowImpl::close() {}

bool lug::Window::priv::WindowImpl::pollEvent(lug::Window::Event& event) {

    if (androidEventQueue.inputQueue != nullptr) {
        AInputEvent *androidEvent = nullptr;
        while (AInputQueue_getEvent(androidEventQueue.inputQueue, &androidEvent) >= 0) {
            LOGI("New input event: type=%d\n", AInputEvent_getType(androidEvent));

            lug::Window::Event e;
            e.type = lug::Window::EventType::INPUT;
            androidEventQueue.events.push(std::move(e));

            if (AInputQueue_preDispatchEvent(androidEventQueue.inputQueue, androidEvent)) {
                continue;
            }
            int32_t handled = 0;
            AInputQueue_finishEvent(androidEventQueue.inputQueue, androidEvent, handled);
        }
    }

    if (!androidEventQueue.events.empty()) {
        event = androidEventQueue.events.front();
        androidEventQueue.events.pop();
        return true;
    }

    return false;
}