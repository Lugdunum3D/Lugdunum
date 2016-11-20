#pragma once

#include <lug/System/Export.hpp>
#include <lug/System/Memory/Area/IArea.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Area {

template <size_t PageSize = 4096, size_t MaxPageCount = 1>
class GrowingHeap : public IArea {
public:
    GrowingHeap();
    GrowingHeap(const GrowingHeap&) = delete;
    GrowingHeap(GrowingHeap&&) = delete;

    GrowingHeap& operator=(const GrowingHeap&) = delete;
    GrowingHeap& operator=(GrowingHeap&&) = delete;

    ~GrowingHeap();

    Page* requestNextPage() override;

private:
    void* _data[MaxPageCount];

    size_t _current{0};
    Page _pages[MaxPageCount];
};

#include <lug/System/Memory/Area/GrowingHeap.inl>

}
}
}
}
