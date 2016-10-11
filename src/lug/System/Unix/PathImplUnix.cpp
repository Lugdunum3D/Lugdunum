#include <lug/System/Unix/PathImplUnix.hpp>

std::string lug::System::Path::getRoot() const {
    return std::string("/");
}

std::string lug::System::Path::getHome() const {
    struct passwd* pw = getpwuid(getuid());
    if (pw == NULL) {
        return std::string("");
    }
    return pw->pw_dir;
}

std::string lug::System::Path::getCwd() const {
    return get_current_dir_name();
}

std::string lug::System::Path::getSave(std::string folderName) const {
    char *basePath = get_current_dir_name();
    struct passwd* pw = getpwuid(getuid());
    if (pw != NULL) {
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

char* lug::System::Path::getEnv(char* variable) const {
    return getenv(variable);
}
