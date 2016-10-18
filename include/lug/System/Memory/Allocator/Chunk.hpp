#pragma once

#include <cmath>
#include <cstddef>
#include <lug/System/Debug.hpp>
#include <lug/System/Export.hpp>
#include <lug/System/Memory/Area/IArea.hpp>
#include <lug/System/Memory/FreeList.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Allocator {

namespace priv {

// std::ceil isn't constexpr in clang and msvc
constexpr int32_t ceil(float num)
{
    return (static_cast<float>(static_cast<int32_t>(num)) == num)
        ? static_cast<int32_t>(num)
        : static_cast<int32_t>(num) + ((num > 0) ? 1 : 0);
}

}

template<size_t MaxSize, size_t MaxAlignment = MaxSize, size_t Offset = 0>
class Chunk {
public:
    Chunk(lug::System::Memory::Area::IArea* area);
    Chunk(const Chunk&) = delete;
    Chunk(Chunk&&) = default;

    Chunk& operator=(const Chunk&) = delete;
    Chunk& operator=(Chunk&&) = default;

    ~Chunk() = default;

    void* allocate(size_t size, size_t alignment, size_t offset);
    void free(void* ptr);
    void reset();

    size_t getSize(void* ptr) const;

private:
    static constexpr size_t ChunkSize = (MaxAlignment <= 1 ? MaxSize : priv::ceil(static_cast<float>(MaxSize) / MaxAlignment) * MaxAlignment);

    lug::System::Memory::Area::IArea* const _area;
    FreeList _freeList{ChunkSize};

    lug::System::Memory::Area::Page* _currentPage{nullptr};
    lug::System::Memory::Area::Page* _firstPage{nullptr};
};

#include <lug/System/Memory/Allocator/Chunk.inl>

}
}
}
}
