#pragma once

#include <lug/Window/Export.hpp>
#include <lug/Window/Keyboard.hpp>

namespace lug {
namespace Window {

enum class LUG_WINDOW_API EventType : uint32_t {
    CLOSE,
    DESTROY,
    KEY_DOWN,
    KEY_UP,
};

struct LUG_WINDOW_API KeyEvent {
    Keyboard::Key code;
    bool alt;
    bool ctrl;
    bool shift;
    bool system;
};

struct LUG_WINDOW_API Event {
    EventType type;

    union {
        KeyEvent key;
    };
};

}
}
