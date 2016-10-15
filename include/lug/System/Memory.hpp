#pragma once

#include <cstddef>
#include <type_traits>
#include <new>

#include <lug/System/Memory/Arena.hpp>
#include <lug/System/Memory/Policies/Thread.hpp>
#include <lug/System/Memory/Policies/BoundsChecker.hpp>

namespace lug {
namespace System {
namespace Memory {

template <typename T, class Arena>
void delete_one(T* object, Arena& arena);

template <typename T, class Arena, typename std::enable_if<!std::is_pod<T>::value, int>::type = 0>
T* new_array(size_t alignment, Arena& arena, size_t nb, const char* file, size_t line);

template <typename T, class Arena, typename std::enable_if<!std::is_pod<T>::value, int>::type = 0>
void delete_array(T* ptr, Arena& arena);

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
T* new_array(size_t alignment, Arena& arena, size_t nb, const char* file, size_t line);

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
void delete_array(T* ptr, Arena& arena);

#include <lug/System/Memory.inl>

}
}
}

#define LUG_NEW_ALIGN(T, alignment, arena) new (arena.allocate(sizeof(T), alignment, __FILE__, __LINE__)) T
#define LUG_NEW_ARRAY_ALIGN(T, alignment, arena) lug::System::Memory::new_array<std::remove_all_extents<T>::type>(alignof(T), arena, std::extent<T>::value, __FILE__, __LINE__)

#define LUG_NEW(T, arena) LUG_NEW_ALIGN(T, alignof(T), arena)
#define LUG_DELETE(object, arena) lug::System::Memory::delete_one(object, arena)

#define LUG_NEW_ARRAY(T, arena) LUG_NEW_ARRAY_ALIGN(T, alignof(std::remove_all_extents<T>::type), arena)
#define LUG_DELETE_ARRAY(object, arena) lug::System::Memory::delete_array(object, arena)

// TODO: provide make_unique / make_shared
