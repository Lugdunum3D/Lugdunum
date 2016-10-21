#include <lug/Main/MainAndroid.hpp>

namespace lug {
namespace Main {

static void shutdown_application(struct lug_android_app* app) {
    pthread_mutex_lock(&app->mutex);
    if (app->savedState != nullptr) {
        free(app->savedState);
        app->savedState = nullptr;
        app->savedStateSize = 0;
    }
    pthread_mutex_unlock(&app->mutex);
}

static void* start_application(void* _app) {
    struct lug_android_app* app = static_cast<struct lug_android_app*>(_app);
    main(0, nullptr);
    shutdown_application(app);
    return nullptr;
}

static struct lug_android_app* lug_app_create(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
    struct lug_android_app* app = new lug_android_app;

    if (savedState != nullptr) {
        app->savedState = malloc(savedStateSize);
        app->savedStateSize = savedStateSize;
        memcpy(app->savedState, savedState, savedStateSize);
    }

    activity->instance = app;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&app->thread, &attr, start_application, app);
    return app;
}

static void onStart(ANativeActivity* activity) {

}

static void onResume(ANativeActivity* activity) {

}

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outSize) {
    return nullptr;
}

static void onPause(ANativeActivity* activity) {

}

static void onStop(ANativeActivity* activity) {

}

static void onDestroy(ANativeActivity* activity) {

}

static void onWindowFocusChanged(ANativeActivity* activity, int hasFocus) {

}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window) {

}

static void onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window) {

}

static void onNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window) {

}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window) {

}


static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue) {

}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue) {

}

static void onContentRectChanged(ANativeActivity* activity, const ARect* rect) {

}

static void onConfigurationChanged(ANativeActivity* activity) {

}

static void onLowMemory(ANativeActivity* activity) {

}

} // Main
} // lug

void  ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
    activity->callbacks->onInputQueueCreated = lug::Main::onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = lug::Main::onInputQueueDestroyed;

    activity->callbacks->onNativeWindowCreated = lug::Main::onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed = lug::Main::onNativeWindowDestroyed;
    activity->callbacks->onNativeWindowResized = lug::Main::onNativeWindowResized;
    activity->callbacks->onNativeWindowRedrawNeeded = lug::Main::onNativeWindowRedrawNeeded;
    activity->callbacks->onContentRectChanged = lug::Main::onContentRectChanged;

    activity->callbacks->onWindowFocusChanged = lug::Main::onWindowFocusChanged;
    activity->callbacks->onConfigurationChanged = lug::Main::onConfigurationChanged;
    activity->callbacks->onLowMemory = lug::Main::onLowMemory;
    activity->callbacks->onStart = lug::Main::onStart;
    activity->callbacks->onResume = lug::Main::onResume;
    activity->callbacks->onSaveInstanceState = lug::Main::onSaveInstanceState;
    activity->callbacks->onPause = lug::Main::onPause;
    activity->callbacks->onStop = lug::Main::onStop;
    activity->callbacks->onDestroy = lug::Main::onDestroy;

    activity->instance = lug::Main::lug_app_create(activity, savedState, savedStateSize);
}
