#include <lug/System/Path.hpp>

#if defined(LUG_SYSTEM_WINDOWS)

    #include <lug/System/Win32/PathImplWin32.hpp>

#elif defined(LUG_SYSTEM_LINUX)

    #include <lug/System/Unix/PathImplUnix.hpp>

#elif defined(LUG_SYSTEM_ANDROID)

    #include <lug/System/Unix/PathImplUnix.hpp>

#endif

std::string lug::System::Path::root() {
    return lug::System::Path::priv::root();
}

std::string lug::System::Path::home() {
    return lug::System::Path::priv::home();
}

std::string lug::System::Path::cwd() {
    return lug::System::Path::priv::cwd();
}

std::string lug::System::Path::save(const std::string& folderName) {
    return lug::System::Path::priv::save(folderName);
}
