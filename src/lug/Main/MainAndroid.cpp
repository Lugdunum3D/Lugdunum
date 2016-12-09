#include <thread>
#include <lug/Main/MainAndroid.hpp>

/* todo : to remove once we have the logger */
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "lugdunum", __VA_ARGS__))

namespace lug {
namespace Main {

std::queue<lug::Window::Event> events;
AInputQueue* inputQueue;

AndroidApp::AndroidApp(ANativeActivity* activity, void* savedState, size_t savedStateSize)
    : _savedState{}, _savedStateSize{} {
    activity->callbacks->onInputQueueCreated = &AndroidApp::onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = &AndroidApp::onInputQueueDestroyed;

    activity->callbacks->onNativeWindowCreated = &AndroidApp::onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed = &AndroidApp::onNativeWindowDestroyed;
    activity->callbacks->onNativeWindowResized = &AndroidApp::onNativeWindowResized;
    activity->callbacks->onNativeWindowRedrawNeeded = &AndroidApp::onNativeWindowRedrawNeeded;
    activity->callbacks->onContentRectChanged = &AndroidApp::onContentRectChanged;

    activity->callbacks->onWindowFocusChanged = &AndroidApp::onWindowFocusChanged;
    activity->callbacks->onConfigurationChanged = &AndroidApp::onConfigurationChanged;
    activity->callbacks->onLowMemory = &AndroidApp::onLowMemory;
    activity->callbacks->onStart = &AndroidApp::onStart;
    activity->callbacks->onResume = &AndroidApp::onResume;
    activity->callbacks->onSaveInstanceState = &AndroidApp::onSaveInstanceState;
    activity->callbacks->onPause = &AndroidApp::onPause;
    activity->callbacks->onStop = &AndroidApp::onStop;
    activity->callbacks->onDestroy = &AndroidApp::onDestroy;

    if (savedState != nullptr) {
        _savedState = ::operator new(savedStateSize);
        _savedStateSize = savedStateSize;
        std::memcpy(_savedState, savedState, savedStateSize);
    }

    std::thread t(&AndroidApp::startApplication, this);
    t.detach();
}

void AndroidApp::shutdowApplication() {
    if (_savedState != nullptr) {
        ::operator delete(_savedState);
        _savedState = nullptr;
        _savedStateSize = 0;
    }
    delete this;
}

void AndroidApp::startApplication() {
    LOGI("startApplication");

    main(0, nullptr);
    shutdowApplication();
}

void AndroidApp::onStart(ANativeActivity*) {
    LOGI("onStart");

    lug::Window::Event e;
    e.type = lug::Window::EventType::START;
    events.push(std::move(e));

}

void AndroidApp::onResume(ANativeActivity*) {
    LOGI("onResume");

    lug::Window::Event e;
    e.type = lug::Window::EventType::RESUME;
    events.push(std::move(e));

}

void *AndroidApp::onSaveInstanceState(ANativeActivity*, size_t*) {
    LOGI("onSaveInstanceState");

    return nullptr;
}

void AndroidApp::onPause(ANativeActivity*) {
    LOGI("onPause");


    lug::Window::Event e;
    e.type = lug::Window::EventType::PAUSE;
    events.push(std::move(e));
}

void AndroidApp::onStop(ANativeActivity*) {
    LOGI("onStop");

    lug::Window::Event e;
    e.type = lug::Window::EventType::STOP;
    events.push(std::move(e));

}

void AndroidApp::onDestroy(ANativeActivity*) {
    LOGI("onDestroy");
    lug::Window::Event e;
    e.type = lug::Window::EventType::DESTROY;
    events.push(std::move(e));

}

void AndroidApp::onWindowFocusChanged(ANativeActivity*, int focus) {
    LOGI("onWindowFocusChanged");
    LOGI(focus ? "APP_CMD_GAINED_FOCUS" : "APP_CMD_LOST_FOCUS");

    lug::Window::Event e;
    e.type = focus ? lug::Window::EventType::FOCUS_GAINED : lug::Window::EventType::FOCUS_LOST;
    events.push(std::move(e));

}

void AndroidApp::onNativeWindowCreated(ANativeActivity*, ANativeWindow* ) {
    LOGI("onNativeWindowCreated");

//    MyandroidEventQueue.window = window;
}

void AndroidApp::onNativeWindowResized(ANativeActivity*, ANativeWindow* ) {
    LOGI("onNativeWindowResized");
}

void AndroidApp::onNativeWindowRedrawNeeded(ANativeActivity*, ANativeWindow*) {
    LOGI("onNativeWindowRedrawNeeded");
}

void AndroidApp::onNativeWindowDestroyed(ANativeActivity*, ANativeWindow* ) {
    LOGI("onNativeWindowDestroyed");
}

void AndroidApp::onInputQueueCreated(ANativeActivity*, AInputQueue* input) {
    LOGI("onInputQueueCreated");

    inputQueue = input;
}

void AndroidApp::onInputQueueDestroyed(ANativeActivity*, AInputQueue*) {
    LOGI("onInputQueueDestroyed");
}

void AndroidApp::onContentRectChanged(ANativeActivity*, const ARect*) {
    LOGI("onContentRectChanged");
}

void AndroidApp::onConfigurationChanged(ANativeActivity*) {
    LOGI("onConfigurationChanged");
}

void AndroidApp::onLowMemory(ANativeActivity*) {
    LOGI("onLowMemory");

}

} // Main
} // lug

void  ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
    LOGI("ANativeActivity_onCreate");
    activity->instance = new lug::Main::AndroidApp(activity, savedState, savedStateSize);
}
