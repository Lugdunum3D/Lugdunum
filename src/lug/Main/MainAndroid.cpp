#include <thread>
#include <lug/Main/MainAndroid.hpp>

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
    main(0, nullptr);
    shutdowApplication();
}

void LugAndroidApp::onStart(ANativeActivity*) {

}

void LugAndroidApp::onResume(ANativeActivity*) {

}

void *LugAndroidApp::onSaveInstanceState(ANativeActivity*, size_t*) {
    return nullptr;
}

void LugAndroidApp::onPause(ANativeActivity*) {

}

void LugAndroidApp::onStop(ANativeActivity*) {

}

void LugAndroidApp::onDestroy(ANativeActivity*) {

}

void LugAndroidApp::onWindowFocusChanged(ANativeActivity*, int) {

}

void LugAndroidApp::onNativeWindowCreated(ANativeActivity*, ANativeWindow*) {

}

void LugAndroidApp::onNativeWindowResized(ANativeActivity*, ANativeWindow*) {

}

void LugAndroidApp::onNativeWindowRedrawNeeded(ANativeActivity*, ANativeWindow*) {

}

void LugAndroidApp::onNativeWindowDestroyed(ANativeActivity*, ANativeWindow*) {

}

void LugAndroidApp::onInputQueueCreated(ANativeActivity*, AInputQueue*) {

}

void LugAndroidApp::onInputQueueDestroyed(ANativeActivity*, AInputQueue*) {

}

void LugAndroidApp::onContentRectChanged(ANativeActivity*, const ARect*) {

}

void LugAndroidApp::onConfigurationChanged(ANativeActivity*) {

}

void LugAndroidApp::onLowMemory(ANativeActivity*) {

}

} // Main
} // lug

void  ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
    activity->instance = new lug::Main::LugAndroidApp(activity, savedState, savedStateSize);
}
