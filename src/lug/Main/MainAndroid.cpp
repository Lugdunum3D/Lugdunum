#include <thread>
#include <lug/Main/MainAndroid.hpp>

/* todo : to remove once we have the logger */
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "lugdunum", __VA_ARGS__))

namespace lug {
namespace Main {

LugAndroidApp::LugAndroidApp(ANativeActivity* activity, void* savedState, size_t savedStateSize)
    : _savedState{}, _savedStateSize{} {
    activity->callbacks->onInputQueueCreated = &LugAndroidApp::onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = &LugAndroidApp::onInputQueueDestroyed;

    activity->callbacks->onNativeWindowCreated = &LugAndroidApp::onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed = &LugAndroidApp::onNativeWindowDestroyed;
    activity->callbacks->onNativeWindowResized = &LugAndroidApp::onNativeWindowResized;
    activity->callbacks->onNativeWindowRedrawNeeded = &LugAndroidApp::onNativeWindowRedrawNeeded;
    activity->callbacks->onContentRectChanged = &LugAndroidApp::onContentRectChanged;

    activity->callbacks->onWindowFocusChanged = &LugAndroidApp::onWindowFocusChanged;
    activity->callbacks->onConfigurationChanged = &LugAndroidApp::onConfigurationChanged;
    activity->callbacks->onLowMemory = &LugAndroidApp::onLowMemory;
    activity->callbacks->onStart = &LugAndroidApp::onStart;
    activity->callbacks->onResume = &LugAndroidApp::onResume;
    activity->callbacks->onSaveInstanceState = &LugAndroidApp::onSaveInstanceState;
    activity->callbacks->onPause = &LugAndroidApp::onPause;
    activity->callbacks->onStop = &LugAndroidApp::onStop;
    activity->callbacks->onDestroy = &LugAndroidApp::onDestroy;

    if (savedState != nullptr) {
        _savedState = ::operator new(savedStateSize);
        _savedStateSize = savedStateSize;
        std::memcpy(_savedState, savedState, savedStateSize);
    }

    std::thread t(&LugAndroidApp::startApplication, this);
    t.detach();
}

void LugAndroidApp::shutdowApplication() {
    if (_savedState != nullptr) {
        ::operator delete(_savedState);
        _savedState = nullptr;
        _savedStateSize = 0;
    }
    delete this;
}

void LugAndroidApp::startApplication() {
    LOGI("startApplication");

    main(0, nullptr);
    shutdowApplication();
}

void LugAndroidApp::onStart(ANativeActivity*) {
    LOGI("onStart");

    lug::Window::Event e;
    e.type = lug::Window::EventType::START;
    androidEventQueue.events.push(std::move(e));

}

void LugAndroidApp::onResume(ANativeActivity*) {
    LOGI("onResume");

    lug::Window::Event e;
    e.type = lug::Window::EventType::RESUME;
    androidEventQueue.events.push(std::move(e));

}

void *LugAndroidApp::onSaveInstanceState(ANativeActivity*, size_t*) {
    LOGI("onSaveInstanceState");

    return nullptr;
}

void LugAndroidApp::onPause(ANativeActivity*) {
    LOGI("onPause");


    lug::Window::Event e;
    e.type = lug::Window::EventType::PAUSE;
    androidEventQueue.events.push(std::move(e));
}

void LugAndroidApp::onStop(ANativeActivity*) {
    LOGI("onStop");

    lug::Window::Event e;
    e.type = lug::Window::EventType::STOP;
    androidEventQueue.events.push(std::move(e));

}

void LugAndroidApp::onDestroy(ANativeActivity*) {
    LOGI("onDestroy");
    lug::Window::Event e;
    e.type = lug::Window::EventType::DESTROY;
    androidEventQueue.events.push(std::move(e));

}

void LugAndroidApp::onWindowFocusChanged(ANativeActivity*, int focus) {
    LOGI("onWindowFocusChanged");
    LOGI(focus ? "APP_CMD_GAINED_FOCUS" : "APP_CMD_LOST_FOCUS");

    lug::Window::Event e;
    e.type = focus ? lug::Window::EventType::FOCUS_GAINED : lug::Window::EventType::FOCUS_LOST;
    androidEventQueue.events.push(std::move(e));

}

void LugAndroidApp::onNativeWindowCreated(ANativeActivity*, ANativeWindow* ) {
    LOGI("onNativeWindowCreated");

//    MyandroidEventQueue.window = window;
}

void LugAndroidApp::onNativeWindowResized(ANativeActivity*, ANativeWindow* ) {
    LOGI("onNativeWindowResized");
}

void LugAndroidApp::onNativeWindowRedrawNeeded(ANativeActivity*, ANativeWindow*) {
    LOGI("onNativeWindowRedrawNeeded");
}

void LugAndroidApp::onNativeWindowDestroyed(ANativeActivity*, ANativeWindow* ) {
    LOGI("onNativeWindowDestroyed");
}

void LugAndroidApp::onInputQueueCreated(ANativeActivity*, AInputQueue* input) {
    LOGI("onInputQueueCreated");

    androidEventQueue.inputQueue = input;
}

void LugAndroidApp::onInputQueueDestroyed(ANativeActivity*, AInputQueue*) {
    LOGI("onInputQueueDestroyed");
}

void LugAndroidApp::onContentRectChanged(ANativeActivity*, const ARect*) {
    LOGI("onContentRectChanged");
}

void LugAndroidApp::onConfigurationChanged(ANativeActivity*) {
    LOGI("onConfigurationChanged");
}

void LugAndroidApp::onLowMemory(ANativeActivity*) {
    LOGI("onLowMemory");

}

} // Main
} // lug

void  ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
    LOGI("ANativeActivity_onCreate");
    activity->instance = new lug::Main::LugAndroidApp(activity, savedState, savedStateSize);
}
