#pragma once

#include <lug/System/Export.hpp>
#include <lug/System/Memory/Area/IArea.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Area {

template <size_t PageSize = 4096, size_t PageCount = 1>
class Heap : public IArea {
public:
    Heap();
    Heap(const Heap&) = delete;
    Heap(Heap&&) = delete;

    Heap& operator=(const Heap&) = delete;
    Heap& operator=(Heap&&) = delete;

    ~Heap();

    Page* requestNextPage() override;

private:
    void* _data{nullptr};

    size_t _current{0};
    Page _pages[PageCount];
};

#include <lug/System/Memory/Area/Heap.inl>

}
}
}
}
