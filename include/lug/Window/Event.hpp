#pragma once

#include <cstdint>

#include <lug/Window/Export.hpp>
#include <lug/Window/Keyboard.hpp>

namespace lug {
namespace Window {

/**
 * @brief      Represents a keyboard event
 */
struct LUG_WINDOW_API KeyEvent {
    Keyboard::Key code; ///< The key that triggered the event
    bool alt;           ///< True if alt was pressed, False otherwise
    bool ctrl;          ///< True if ctrl was pressed, False otherwise
    bool shift;         ///< True if shift was pressed, False otherwise
    bool system;        ///< True if system was pressed, False otherwise
};

/**
 * @brief      Represents a char event
 */
struct LUG_WINDOW_API CharEvent {
    wchar_t val;        ///< Raw value of the character pressed (with accents, etc.)
};

/**
 * @brief      Represents an event
 */
struct LUG_WINDOW_API Event {

    /**
     * Type of event
     */
    enum class LUG_WINDOW_API Type : uint32_t {
        Close,          ///< Window close event
        Destroy,        ///< Window destroy event
	Resize,         ///< Window resize event
        KeyPressed,     ///< KeyPressed event
        KeyReleased,    ///< KeyReleased event
        CharEntered,    ///< CharEntered event
    };

    Type type;          ///< The type of the event

    /**
     * Value of the event
     */
    union {
        KeyEvent key;           ///< A KeyEvent
        CharEvent character;    ///< A CharEvent
    };
};

} // Window
} // lug
