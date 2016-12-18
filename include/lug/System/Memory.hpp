#pragma once

#include <cstddef>
#include <type_traits>
#include <new>
#include <memory>
#include <functional>

#include <lug/System/Memory/Arena.hpp>
#include <lug/System/Memory/Policies/Thread.hpp>
#include <lug/System/Memory/Policies/BoundsChecker.hpp>
#include <lug/System/Memory/Policies/MemoryMarker.hpp>


// The variadic arguments are always "arena, args..." (args only work for non array allocation and array of non pod)
#define LUG_NEW_ALIGN(T, alignment, ...) ::lug::System::Memory::new_one<T>(alignment, __FILE__, __LINE__, __VA_ARGS__)
#define LUG_NEW_ARRAY_ALIGN(T, alignment, ...) LUG_NEW_ARRAY_ALIGN_SIZE(typename std::remove_all_extents<T>::type, alignment, std::extent<T>::value, __VA_ARGS__)
#define LUG_NEW_ARRAY_ALIGN_SIZE(T, alignment, size, ...) ::lug::System::Memory::new_array<typename std::remove_all_extents<T>::type>(alignment, size, __FILE__, __LINE__, __VA_ARGS__)

#define LUG_NEW(T, ...) LUG_NEW_ALIGN(T, alignof(T), __VA_ARGS__)
#define LUG_DELETE(object, arena) ::lug::System::Memory::delete_one(object, arena)

#define LUG_NEW_ARRAY(T, ...) LUG_NEW_ARRAY_ALIGN(T, alignof(typename std::remove_all_extents<T>::type), __VA_ARGS__)
#define LUG_NEW_ARRAY_SIZE(T, size, ...) LUG_NEW_ARRAY_ALIGN_SIZE(T, alignof(typename std::remove_all_extents<T>::type), size, __VA_ARGS__)
#define LUG_DELETE_ARRAY(object, arena) ::lug::System::Memory::delete_array(object, arena)


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

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
T* new_array(size_t alignment, size_t nb, const char* file, size_t line, Arena& arena);

template <typename T, class Arena, class ...Args, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
T* new_array(size_t alignment, size_t nb, const char* file, size_t line, Arena& arena, Args&&... args) = delete;

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
void delete_array(T* ptr, Arena& arena);

// unique_ptr
namespace priv {

template <typename T>
struct make_unique_deleter {
    using Deleter = std::function<void (T*)>;
};

template <typename T>
struct make_unique_deleter<T[]> {
    using Deleter = std::function<void (T*)>;
};

template <typename T, size_t Count>
struct make_unique_deleter<T[Count]> {
    using Deleter = std::function<void (T*)>;
};

} // namespace priv

template<typename T, typename Deleter = typename priv::make_unique_deleter<T>::Deleter>
using unique_ptr = std::unique_ptr<T, Deleter>;

namespace priv {

template <typename T>
struct make_unique_if {
    using SingleObject = lug::System::Memory::unique_ptr<T>;
};

template <typename T>
struct make_unique_if<T[]> {
    using UnknownBound = lug::System::Memory::unique_ptr<T[]>;
};

template <typename T, size_t Count>
struct make_unique_if<T[Count]> {
    using KnownBound = lug::System::Memory::unique_ptr<T[]>;
};

} // namespace priv

// Single object
template <typename T, class Arena, typename ...Args>
typename priv::make_unique_if<T>::SingleObject make_unique(Arena& arena, Args&&... args);

template <typename T, class Arena, typename ...Args>
typename priv::make_unique_if<T>::SingleObject make_unique_align(Arena& arena, size_t alignment, Args&&... args);

// Dynamic array
template <typename T, class Arena, typename ...Args, typename std::enable_if<!std::is_pod<T>::value, int>::type = 0>
typename priv::make_unique_if<T>::UnknownBound make_unique(Arena& arena, size_t size, Args&&... args);

template <typename T, class Arena, typename ...Args, typename std::enable_if<!std::is_pod<T>::value, int>::type = 0>
typename priv::make_unique_if<T>::UnknownBound make_unique_align(Arena& arena, size_t alignment, size_t size, Args&&... args);

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
typename priv::make_unique_if<T>::UnknownBound make_unique(Arena& arena, size_t size);

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
typename priv::make_unique_if<T>::UnknownBound make_unique_align(Arena& arena, size_t alignment, size_t size);

template <typename T, class Arena, typename ...Args, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
typename priv::make_unique_if<T>::UnknownBound make_unique(Arena& arena, size_t size, Args&&... args) = delete;

template <typename T, class Arena, typename ...Args, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
typename priv::make_unique_if<T>::UnknownBound make_unique_align(Arena& arena, size_t alignment, size_t size, Args&&... args) = delete;

// Static array
template <typename T, class Arena, typename ...Args, typename std::enable_if<!std::is_pod<T>::value, int>::type = 0>
typename priv::make_unique_if<T>::KnownBound make_unique(Arena& arena, Args&&... args);

template <typename T, class Arena, typename ...Args, typename std::enable_if<!std::is_pod<T>::value, int>::type = 0>
typename priv::make_unique_if<T>::KnownBound make_unique_align(Arena& arena, size_t alignment, Args&&... args);

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
typename priv::make_unique_if<T>::KnownBound make_unique(Arena& arena);

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
typename priv::make_unique_if<T>::KnownBound make_unique_align(Arena& arena, size_t alignment);

template <typename T, class Arena, typename ...Args, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
typename priv::make_unique_if<T>::KnownBound make_unique(Arena& arena, Args&&... args) = delete;

template <typename T, class Arena, typename ...Args, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
typename priv::make_unique_if<T>::KnownBound make_unique_align(Arena& arena, size_t alignment, Args&&... args) = delete;

// TODO: Develop shared_ptr too

#include <lug/System/Memory.inl>

} // Memory
} // System
} // lug
