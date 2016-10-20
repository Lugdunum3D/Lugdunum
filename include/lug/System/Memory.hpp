#pragma once

#include <cstddef>
#include <type_traits>
#include <new>

#include <lug/System/Memory/Arena.hpp>
#include <lug/System/Memory/Policies/Thread.hpp>
#include <lug/System/Memory/Policies/BoundsChecker.hpp>
#include <lug/System/Memory/Policies/MemoryMarker.hpp>

namespace lug {
namespace System {
namespace Memory {

template <typename T, class Arena, class ...Args>
T* new_one(size_t alignment, const char* file, size_t line, Arena& arena, Args&&... args);

template <typename T, class Arena>
void delete_one(T* object, Arena& arena);

template <typename T, class Arena, class ...Args, typename std::enable_if<!std::is_pod<T>::value, int>::type = 0>
T* new_array(size_t alignment, size_t nb, const char* file, size_t line, Arena& arena, Args&&... args);

template <typename T, class Arena, typename std::enable_if<!std::is_pod<T>::value, int>::type = 0>
void delete_array(T* ptr, Arena& arena);

template <typename T, class Arena, class ...Args, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
T* new_array(size_t alignment, size_t nb, const char* file, size_t line, Arena& arena, Args&&... args);

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
void delete_array(T* ptr, Arena& arena);

#include <lug/System/Memory.inl>

}
}
}

// The variadic arguments are always "arena, args..." (args only work for non array allocation and array of non pod)
#define LUG_NEW_ALIGN(T, alignment, ...) ::lug::System::Memory::new_one<T>(alignment, __FILE__, __LINE__, __VA_ARGS__)
#define LUG_NEW_ARRAY_ALIGN(T, alignment, ...) LUG_NEW_ARRAY_ALIGN_SIZE(typename std::remove_all_extents<T>::type, alignment, std::extent<T>::value, __VA_ARGS__)
#define LUG_NEW_ARRAY_ALIGN_SIZE(T, alignment, size, ...) ::lug::System::Memory::new_array<std::remove_all_extents<T>::type>(alignment, size, __FILE__, __LINE__, __VA_ARGS__)

#define LUG_NEW(T, ...) LUG_NEW_ALIGN(T, alignof(T), __VA_ARGS__)
#define LUG_DELETE(object, arena) ::lug::System::Memory::delete_one(object, arena)

#define LUG_NEW_ARRAY(T, ...) LUG_NEW_ARRAY_ALIGN(T, alignof(std::remove_all_extents<T>::type), __VA_ARGS__)
#define LUG_NEW_ARRAY_SIZE(T, size, ...) LUG_NEW_ARRAY_ALIGN_SIZE(T, alignof(std::remove_all_extents<T>::type), size, __VA_ARGS__)
#define LUG_DELETE_ARRAY(object, arena) ::lug::System::Memory::delete_array(object, arena)

// TODO: provide make_unique / make_shared
