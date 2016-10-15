#include <lug/System/Unix/PathImplUnix.hpp>

#include <pwd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <utility>
#include <unistd.h>

/*
    Return Unix root
    Example: /
*/
std::string lug::System::Path::priv::root() {
    return std::string("/");
}

/*
    Return Unix home
    Example: /home/${USER}
*/
std::string lug::System::Path::priv::home() {
    struct passwd* pw = getpwuid(getuid());
    if (pw == nullptr) {
        return std::string("");
    }
    return pw->pw_dir;
}

/*
    Return the current working directory
    Example: /home/${USER}/vulkan/lugdunum
*/
std::string lug::System::Path::priv::cwd() {
    char* cwd = get_current_dir_name();
    if (cwd == nullptr) {
        return std::string("");
    }
    return cwd;
}

/*
    Return the path of saves folder
    Example: /home/${USER}/.lug
*/
std::string lug::System::Path::priv::save(const std::string& folderName) {
    std::string basePath = lug::System::Path::priv::cwd();

    struct passwd* pw = getpwuid(getuid());
    if (pw != nullptr) {
        basePath = pw->pw_dir;
    }

    return basePath + "/" + folderName;
}
