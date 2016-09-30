#pragma once

// Detect the operating system
#if defined(_WIN32)

    #define LUG_SYSTEM_WINDOWS

#elif defined(__unix__)

    #if defined(__linux__)

        #define LUG_SYSTEM_LINUX

    #else

        #error This UNIX operating system is not supported yet

    #endif

#else

    #error This operating system is not supported yet

#endif

// Define a portable debug macro
#if !defined(NDEBUG)

    #define LUG_DEBUG

#endif

// Define macros for import / export
#if !defined(LUG_STATIC)

    #if defined(LUG_SYSTEM_WINDOWS)

        #define LUG_API_EXPORT __declspec(dllexport)
        #define LUG_API_IMPORT __declspec(dllimport)

    #else

        #define LUG_API_EXPORT __attribute__ ((__visibility__ ("default")))
        #define LUG_API_IMPORT __attribute__ ((__visibility__ ("default")))

    #endif

#else

    #define LUG_API_EXPORT
    #define LUG_API_IMPORT

#endif
