#include <lug/System/Path.hpp>

#if defined(LUG_SYSTEM_WINDOWS)

    #include <lug/System/Win32/PathImplWin32.hpp>

#elif defined(LUG_SYSTEM_LINUX)

    #include <lug/System/Unix/PathImplUnix.hpp>

#elif defined(LUG_SYSTEM_ANDROID)

    #include <lug/System/Unix/PathImplUnix.hpp>

#endif

std::string lug::System::Path::getRoot() {
    return lug::System::Path::priv::getRoot();
}

std::string lug::System::Path::getHome() {
    return lug::System::Path::priv::getHome();
}

std::string lug::System::Path::getCwd() {
    return lug::System::Path::priv::getCwd();
}

std::string lug::System::Path::getSave(std::string folderName) {
    return lug::System::Path::priv::getSave(folderName);
}
