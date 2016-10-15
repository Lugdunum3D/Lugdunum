#include <lug/System/Unix/PathImplUnix.hpp>

#include <pwd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

std::string lug::System::Path::priv::root() {
    return std::string("/");
}

std::string lug::System::Path::priv::home() {
    struct passwd* pw = getpwuid(getuid());
    if (pw == nullptr) {
        return std::string("");
    }
    return pw->pw_dir;
}

std::string lug::System::Path::priv::cwd() {
    char* cwd = get_current_dir_name()
    if (cwd == nullptr) {
        return std::string("");
    }
    return cwd;
}

std::string lug::System::Path::priv::save(const std::string& folderName) {
    char* basePath = lug::System::Path::priv::cwd().c_str();

    struct passwd* pw = getpwuid(getuid());
    if (pw != nullptr) {
        basePath = pw->pw_dir;
    }

    std::string savePath(basePath);
    savePath += "/";
    savePath += folderName;

    return savePath;
}
