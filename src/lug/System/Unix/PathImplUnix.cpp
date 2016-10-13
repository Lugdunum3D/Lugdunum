#include <lug/System/Unix/PathImplUnix.hpp>

#include <pwd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

std::string lug::System::Path::priv::getRoot() {
    return std::string("/");
}

std::string lug::System::Path::priv::getHome() {
    struct passwd* pw = getpwuid(getuid());
    if (pw == nullptr) {
        return std::string("");
    }
    return pw->pw_dir;
}

std::string lug::System::Path::priv::getCwd() {
    return get_current_dir_name();
}

std::string lug::System::Path::priv::getSave(std::string folderName) {
    char* basePath = get_current_dir_name();

    struct passwd* pw = getpwuid(getuid());
    if (pw != nullptr) {
        basePath = pw->pw_dir;
    }

    std::string savePath(basePath);
    savePath += "/";
    savePath += folderName;

    struct stat st;
    if (stat(savePath.c_str(), &st) != 0) {
        mkdir(savePath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    return savePath;
}

char* lug::System::Path::priv::getEnv(char* variable) {
    return getenv(variable);
}
