#pragma once

#include <lug/System/Export.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Area {

// A memory page where `end` is included (i.e. `end` is the last byte in the page)
// The areas have to set `prev` to the previous page's address and `next` to `nullptr` before returning a `Page`
struct LUG_SYSTEM_API Page {
    void* start{nullptr};
    void* end{nullptr};

    Page* prev{nullptr};
    Page* next{nullptr};
};

class LUG_SYSTEM_API IArea {
public:
    IArea() = default;
    IArea(const IArea&) = delete;
    IArea(IArea&&) = delete;

    IArea& operator=(const IArea&) = delete;
    IArea& operator=(IArea&&) = delete;

    virtual ~IArea() = default;

    virtual Page* requestNextPage() = 0;
};

} // Area
} // Memory
} // System
} // lug
