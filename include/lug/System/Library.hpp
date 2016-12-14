#pragma once

#include <lug/System/Export.hpp>

#if defined(LUG_SYSTEM_WINDOWS)
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

namespace lug {
namespace System {
namespace Library {

#if defined(LUG_SYSTEM_WINDOWS)
using Handle = HMODULE;
#else
using Handle = void*;
#endif

Handle open(const char* name);
void close(Handle handle);

template<typename Function>
Function sym(Handle handle, const char *name);

#include <lug/System/Library.inl>

}
}
}
