#pragma once

// Info about Lugdunum
#define LUG_NAME "Lugdunum3D"

#define LUG_VERSION_MAJOR 0
#define LUG_VERSION_MINOR 1
#define LUG_VERSION_PATCH 0

// Detect the operating system
#if defined(_WIN32)
    #define LUG_SYSTEM_WINDOWS
#elif defined(__unix__)
    #if defined(__ANDROID__)
        #define LUG_SYSTEM_ANDROID
    #elif defined(__linux__)
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

#if defined(__clang__)
    #define LUG_COMPILER_CLANG
#elif defined(__GNUC__)
    #define LUG_COMPILER_GCC
#elif defined(_MSC_VER)
    #define LUG_COMPILER_MSVC
#endif


// Define macros for import / export
#if defined(LUG_SYSTEM_WINDOWS)
    #define LUG_API_EXPORT __declspec(dllexport)
    #define LUG_API_IMPORT __declspec(dllimport)

    // Disable C4251 and C4275 warnings for Visual C++ compilers
    #if defined(LUG_COMPILER_MSVC)
        #pragma warning(disable:4251)
        #pragma warning(disable:4275)
    #endif
#else
    #define LUG_API_EXPORT __attribute__ ((__visibility__ ("default")))
    #define LUG_API_IMPORT __attribute__ ((__visibility__ ("default")))
#endif

