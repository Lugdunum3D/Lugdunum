#include <thread>
#include <mutex>
#include <lug/Main/MainAndroid.hpp>
#include <lug/Window/Android/WindowImplAndroid.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Main {

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

void AndroidApp::onStart(ANativeActivity*) {
    LUG_LOG.info("onStart");

}

void AndroidApp::onResume(ANativeActivity*) {
    LUG_LOG.info("onResume");

}

void* AndroidApp::onSaveInstanceState(ANativeActivity*, size_t*) {
    LUG_LOG.info("onSaveInstanceState");

    return nullptr;
}

void AndroidApp::onPause(ANativeActivity*) {
    LUG_LOG.info("onPause");

}

void AndroidApp::onStop(ANativeActivity*) {
    LUG_LOG.info("onStop");

}

void AndroidApp::onDestroy(ANativeActivity* ) {
    LUG_LOG.info("onDestroy");

    std::lock_guard<std::mutex> lock(lug::Window::priv::WindowImpl::androidMutex);
    lug::Window::Event e;
    e.type = lug::Window::Event::Type::AndroidDestroy;
    lug::Window::priv::WindowImpl::events.push(std::move(e));
}

void AndroidApp::onWindowFocusChanged(ANativeActivity*, int) {
    LUG_LOG.info("onWindowFocusChanged");

}

void AndroidApp::onNativeWindowCreated(ANativeActivity*, ANativeWindow* window) {
    LUG_LOG.info("onNativeWindowCreated");
    std::lock_guard<std::mutex> lk(lug::Window::priv::WindowImpl::androidMutex);
    lug::Window::priv::WindowImpl::nativeWindow = window;
    lug::Window::priv::WindowImpl::cv.notify_one();
}

void AndroidApp::onNativeWindowResized(ANativeActivity*, ANativeWindow* ) {
    LUG_LOG.info("onNativeWindowResized");
    std::lock_guard<std::mutex> lock(lug::Window::priv::WindowImpl::androidMutex);
    lug::Window::priv::WindowImpl::events.push({lug::Window::Event::Type::Resize});    
}


void AndroidApp::onNativeWindowRedrawNeeded(ANativeActivity*, ANativeWindow*) {
    LUG_LOG.info("onNativeWindowRedrawNeeded");    
}

void AndroidApp::onNativeWindowDestroyed(ANativeActivity*, ANativeWindow* window) {
    LUG_LOG.info("onNativeWindowDestroyed");    

    std::lock_guard<std::mutex> lock(lug::Window::priv::WindowImpl::androidMutex);
    lug::Window::priv::WindowImpl::nativeWindow = window;
}

void AndroidApp::onInputQueueCreated(ANativeActivity*, AInputQueue* input) {
    LUG_LOG.info("onInputQueueCreated");    

    std::lock_guard<std::mutex> lock(lug::Window::priv::WindowImpl::androidMutex);
    lug::Window::priv::WindowImpl::inputQueue = input;
}

void AndroidApp::onInputQueueDestroyed(ANativeActivity*, AInputQueue*) {
    LUG_LOG.info("onInputQueueDestroyed");    

    std::lock_guard<std::mutex> lock(lug::Window::priv::WindowImpl::androidMutex);
    lug::Window::priv::WindowImpl::inputQueue = nullptr;
}

void AndroidApp::onContentRectChanged(ANativeActivity*, const ARect*) {
    LUG_LOG.info("onContentRectChanged");    

}

void AndroidApp::onConfigurationChanged(ANativeActivity*) {
    LUG_LOG.info("onConfigurationChanged");    

}

void AndroidApp::onLowMemory(ANativeActivity*) {
    LUG_LOG.info("onLowMemory");    

}

} // Main
} // lug

void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
    std::lock_guard<std::mutex> lock(lug::Window::priv::WindowImpl::androidMutex);
    activity->instance = new lug::Main::AndroidApp(activity, savedState, savedStateSize);
    lug::Window::priv::WindowImpl::activity = activity;
}
