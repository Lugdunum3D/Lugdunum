#pragma once

#include <lug/Window/Export.hpp>
#include <lug/Window/Keyboard.hpp>

#include <cstdint>

namespace lug {
namespace Window {

/**
 * Type of event
 */
enum class LUG_WINDOW_API EventType : uint32_t {
    Close,          ///< Window close event
    Destroy,        ///< Window destroy event
    KeyPressed,     ///< KeyPressed event
    KeyReleased,    ///< KeyReleased event
    CharEntered,    ///< CharEntered event
};

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
    wchar_t val;        ///< Raw value of the text pressed
};

/**
 * @brief      Represents an event
 */
struct LUG_WINDOW_API Event {
    EventType type;             ///< The type of the event

    /**
     * Value of the event
     */
    union {
        KeyEvent key;           ///< A KeyEvent
        CharEvent character;    ///< A CharEvent
    };
};

}
}
