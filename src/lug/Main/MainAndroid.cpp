#include <thread>
#include <lug/Main/MainAndroid.hpp>

namespace lug {
namespace Main {

LugAndroidApp::LugAndroidApp(ANativeActivity* activity, void* savedState, size_t savedStateSize)
    : _activity{activity}, _savedState{}, _savedStateSize{} {
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
        memcpy(_savedState, savedState, savedStateSize);
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

void LugAndroidApp::onStart(ANativeActivity* activity) {

}

void LugAndroidApp::onResume(ANativeActivity* activity) {

}

void *LugAndroidApp::onSaveInstanceState(ANativeActivity* activity, size_t* outSize) {
    return nullptr;
}

void LugAndroidApp::onPause(ANativeActivity* activity) {

}

void LugAndroidApp::onStop(ANativeActivity* activity) {

}

void LugAndroidApp::onDestroy(ANativeActivity* activity) {

}

void LugAndroidApp::onWindowFocusChanged(ANativeActivity* activity, int hasFocus) {

}

void LugAndroidApp::onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window) {

}

void LugAndroidApp::onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window) {

}

void LugAndroidApp::onNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window) {

}

void LugAndroidApp::onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window) {

}


void LugAndroidApp::onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue) {

}

void LugAndroidApp::onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue) {

}

void LugAndroidApp::onContentRectChanged(ANativeActivity* activity, const ARect* rect) {

}

void LugAndroidApp::onConfigurationChanged(ANativeActivity* activity) {

}

void LugAndroidApp::onLowMemory(ANativeActivity* activity) {

}

} // Main
} // lug

void  ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
    activity->instance = new lug::Main::LugAndroidApp(activity, savedState, savedStateSize);
}
