#include <lug/System/Path.hpp>

#if defined(LUG_SYSTEM_WINDOWS)

    #include <lug/Window/Win32/PathImplWin32.hpp>

#elif defined(LUG_SYSTEM_LINUX)

    #include <lug/Window/Unix/PathImplUnix.hpp>

#elif defined(LUG_SYSTEM_ANDROID)

    #include <lug/Window/Win32/PathImplUnix.hpp>

#endif

lug::System::Path::Path() : _impl(new lug::System::priv::PathImpl()) {}

lug::System::Path::~Path() {
    if (_impl != nullptr) {
        delete _impl;
    }
}

std::string lug::System::Path::getRoot() const {
    return _impl->getRoot();
}
std::string lug::System::Path::getHome() const{
    return _impl->getHome();
}
std::string lug::System::Path::getCwd() const {
    return _impl->getCwd();
}
std::string lug::System::Path::getSave() const {
    return _impl->getSave();
}
