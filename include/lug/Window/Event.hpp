#pragma once

#include <lug/Window/Export.hpp>

namespace lug {
namespace Window {

enum class LUG_WINDOW_API EventType : uint32_t {
    CLOSE,
    DESTROY,
};

struct LUG_WINDOW_API Event {
    EventType type;
};

} // Window
} // lug
