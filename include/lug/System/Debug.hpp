#pragma once

#include <lug/Config.hpp>
#include <lug/System/Utils.hpp>
#include <iostream>


#if defined(LUG_DEBUG)

    #if defined(LUG_COMPILER_GCC) || defined(LUG_COMPILER_CLANG)

        #if defined(LUG_SYSTEM_ANDROID)
            // see https://github.com/scottt/debugbreak/blob/master/debugbreak.h#L90
            #define LUG_DEBUG_BREAK() __asm__ volatile(".inst 0xe7f001f0");
        #else
            #define LUG_DEBUG_BREAK() __asm__("int $3")
        #endif

    #elif defined(LUG_COMPILER_MSVC)

        #define LUG_DEBUG_BREAK() __debugbreak()

    #endif


    #define LUG_ASSERT(assertion, message)\
    do {\
        if (LUG_UNLIKELY(!(assertion))) {\
            std::cerr << "Assertion failed: " << (message) << std::endl <<\
            "In " << __FILE__ << std::endl <<\
            " at `" << LUG_SYSTEM_FUNCTION_NAME << "` line " << __LINE__ << std::endl;\
            LUG_DEBUG_BREAK();\
        }\
    } while (0)

#else

    #define LUG_ASSERT(assertion, message)
    #define LUG_DEBUG_BREAK()

#endif
