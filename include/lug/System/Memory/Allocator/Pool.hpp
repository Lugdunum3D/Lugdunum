#pragma once

#include <lug/System/Memory/Allocator/Chunk.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Allocator {

template <typename T, size_t Alignment = alignof(T), size_t Offset = 0>
using Pool = Chunk<sizeof(T), Alignment, Offset>;

}
}
}
}
