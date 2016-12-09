#include <thread>
#include <lug/Main/MainAndroid.hpp>

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
    main(0, nullptr);
    shutdowApplication();
}

void AndroidApp::onStart(ANativeActivity*) {}

void AndroidApp::onResume(ANativeActivity*) {}

void *AndroidApp::onSaveInstanceState(ANativeActivity*, size_t*) {
    return nullptr;
}

void AndroidApp::onPause(ANativeActivity*) {}

void AndroidApp::onStop(ANativeActivity*) {}

void AndroidApp::onDestroy(ANativeActivity*) {
    lug::Window::Event e;
    e.type = lug::Window::EventType::CLOSE;
    events.push(std::move(e));
}

void AndroidApp::onWindowFocusChanged(ANativeActivity*, int) {}

void AndroidApp::onNativeWindowCreated(ANativeActivity*, ANativeWindow* ) {}

void AndroidApp::onNativeWindowResized(ANativeActivity*, ANativeWindow* ) {}

void AndroidApp::onNativeWindowRedrawNeeded(ANativeActivity*, ANativeWindow*) {}

void AndroidApp::onNativeWindowDestroyed(ANativeActivity*, ANativeWindow* ) {}

void AndroidApp::onInputQueueCreated(ANativeActivity*, AInputQueue* input) {
    inputQueue = input;
}

void AndroidApp::onInputQueueDestroyed(ANativeActivity*, AInputQueue*) {}

void AndroidApp::onContentRectChanged(ANativeActivity*, const ARect*) {}

void AndroidApp::onConfigurationChanged(ANativeActivity*) {}

void AndroidApp::onLowMemory(ANativeActivity*) {}

} // Main
} // lug

void  ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
    activity->instance = new lug::Main::AndroidApp(activity, savedState, savedStateSize);
}
