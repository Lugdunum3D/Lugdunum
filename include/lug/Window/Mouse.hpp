#pragma once

#include <lug/Window/Export.hpp>
#if defined(LUG_SYSTEM_WINDOWS)
#include <Windows.h>
#elif defined(LUG_SYSTEM_LINUX)
#include <X11/keysym.h>
#else
// Insert error message here
#endif

namespace lug {
namespace Window {
namespace Mouse {

/**
* @brief      Abstraction of Mouse buttons
*/
enum class LUG_WINDOW_API Button : int {
    Unknown,    // Unhandled button

    Left,       // The left mouse button
    Right,      // The right mouse button
    Middle,     // The middle (wheel) mouse button
    XButton1,   // The first extra mouse button
    XButton2    // The second extra mouse button
};

} // namespace Mouse
} // namespace Window
} // namespace lug
