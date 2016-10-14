#pragma once

#include <lug/Config.hpp>

namespace lug {
namespace Window {

#if defined(LUG_SYSTEM_WINDOWS)

    struct HWND__;

    using WindowHandle = HWND__*;

#elif defined(LUG_SYSTEM_LINUX) || defined(LUG_SYSTEM_FREEBSD)

    using WindowHandle = unsigned long;

#elif defined(LUG_SYSTEM_ANDROID)

    using WindowHandle = void*;

#endif

} // Window
} // namespace
