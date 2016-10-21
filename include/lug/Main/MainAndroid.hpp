#pragma once

#include <android/native_activity.h>
#include <android/configuration.h>
#include <pthread.h>
#include <lug/Main/Export.hpp>

extern int main(int argc, const char* argv[]);

namespace lug {
namespace Main {

struct lug_android_app {
    ANativeActivity* activity;
    ANativeWindow* window;

    ALooper* looper;
    AInputQueue* inputQueue;
    AConfiguration* config;

    void* savedState;
    size_t savedStateSize;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_t thread;

    AInputQueue* pendingInputQueue;
    ANativeWindow* pendingWindow;
};

} // Main
} // lug
