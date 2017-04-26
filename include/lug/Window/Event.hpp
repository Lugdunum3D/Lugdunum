#pragma once

#include <cstdint>

#include <lug/Window/Export.hpp>
#include <lug/Window/Keyboard.hpp>
#include <lug/Window/Mouse.hpp>
#include <lug/Math/Vector.hpp>

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
* @brief      Represents a mouse event
*/
struct LUG_WINDOW_API MouseEvent {

    union {
        Mouse::Button code; ///< The button that triggered the event
        struct {
            double xOffset;
            double yOffset;
        } scrollOffset;     ///< The distance in x and y that the scroll wheel has traveled
    };
    struct {
        int32_t x;
        int32_t y;
    } coord;        ///< The mouse coordinate if the event was triggered by movement
    bool ctrl;      ///< True if ctrl was pressed, False otherwise
    bool shift;     ///< True if shift was pressed, False otherwise
    bool left;      ///< True if left mouse button was pressed, False otherwise
    bool right;    ///< True if right mouse button was pressed, False otherwise
    bool middle;    ///< True if middle mouse button was pressed, False otherwise
    bool x1;        ///< True if extra 1 mouse button was pressed, False otherwise
    bool x2;        ///< True if extra 2 mouse button was pressed, False otherwise
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
        ButtonPressed,  ///< ButtonPressed event
        ButtonReleased, ///< ButtonReleased event
        MouseMoved,     ///< MouseMoved event
        MouseWheel,     ///< MouseWheelRotated event
        MouseLeave,     ///< Mouse left window event
        MouseEnter      ///< Mouse entered window event
    };

    Type type;          ///< The type of the event

    /**
     * Value of the event
     */
    union {
        KeyEvent key;           ///< A KeyEvent
        CharEvent character;    ///< A CharEvent
        MouseEvent mouse;      ///< A MouseEvent
    };
};

} // Window
} // lug
