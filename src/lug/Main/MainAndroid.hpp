#pragma once

#include <android/configuration.h>
#include <android/native_activity.h>

extern int main(int argc, const char* argv[]);

namespace lug {
namespace Main {

class AndroidApp {
public:
    AndroidApp(ANativeActivity* activity, void* savedState, size_t savedStateSize);

    // Callback functions as defined in android/native_activity.h, must be public
    static void onStart(ANativeActivity* activity);
    static void onResume(ANativeActivity* activity);
    static void* onSaveInstanceState(ANativeActivity* activity, size_t* outSize);
    static void onPause(ANativeActivity* activity);
    static void onStop(ANativeActivity* activity);
    static void onDestroy(ANativeActivity* activity);
    static void onWindowFocusChanged(ANativeActivity* activity, int hasFocus);
    static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window);
    static void onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window);
    static void onNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window);
    static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window);
    static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue);
    static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue);
    static void onContentRectChanged(ANativeActivity* activity, const ARect* rect);
    static void onConfigurationChanged(ANativeActivity* activity);
    static void onLowMemory(ANativeActivity* activity);

private:
    void shutdowApplication();
    void startApplication();

    void* _savedState;
    size_t _savedStateSize;
    // void* window;
};

} // Main
} // lug
