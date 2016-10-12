#if defined(__ANDROID__)
 #include <android/log.h>
 #define LOGV(...)  ((void)__android_log_print(ANDROID_LOG_VERBOSE, "lugdunum sample", __VA_ARGS__))
 #include "android_native_app_glue.h"
 #include "Lug/System/Exception.hpp"
#endif

#if defined(__ANDROID__)
    void android_main(android_app* app)
#else
    int main()
#endif
{
#if defined(__ANDROID__)
    app_dummy();
    while (1)
    {
        int ident;
        int events;
        struct android_poll_source *source;

        while ((ident = ALooper_pollAll(-1, nullptr, &events,
                                        (void **) &source)) >= 0)
        {

            if (source != NULL)
            {
                source->process(app, source);
            }
            if (app->destroyRequested != 0)
            {
                return;
            }

        }
    }
#else
    return 0;
#endif
}
