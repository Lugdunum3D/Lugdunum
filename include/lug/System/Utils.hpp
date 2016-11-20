#pragma once


#if defined(LUG_COMPILER_MSVC)
    #define LUG_SYSTEM_FUNCTION_NAME __FUNCSIG__
#else
    #define LUG_SYSTEM_FUNCTION_NAME __PRETTY_FUNCTION__
#endif


#if defined(__clang__) || defined(__GNUC__)

    #define LUG_LIKELY(x) __builtin_expect(!!(x), 1)
    #define LUG_UNLIKELY(x) __builtin_expect(!!(x), 0)

#else

    #define LUG_LIKELY(x) (x)
    #define LUG_UNLIKELY(x) (x)

#endif
