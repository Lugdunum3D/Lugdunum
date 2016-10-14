#pragma once

#include <lug/System/Export.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Area {

// A page of memory with end include in the page
struct LUG_SYSTEM_API Page {
    void *start;
    void *end;

    Page *next;
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

}
}
}
}
