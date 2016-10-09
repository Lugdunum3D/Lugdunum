#include <lug/System/PathLinux.hpp>

std::string lug::System::PathLinux::getRoot() const {
    return std::string("/");
}

std::string lug::System::PathLinux::getHome() const {
    struct passwd *pw = getpwuid(getuid());
    if (pw == NULL) {
        return std::string("");
    }
    return pw->pw_dir;
}

std::string lug::System::PathLinux::getCwd() const {
    return get_current_dir_name();
}

std::string lug::System::PathLinux::getSave() const {
    struct passwd *pw = getpwuid(getuid());
    char *homePath = get_current_dir_name();
    if (pw != NULL) {
        homePath = pw->pw_dir;
    }

    std::string savePath(homePath);
    savePath += "/.lug";

    struct stat st;
    if (stat(savePath.c_str(), &st) != 0) {
        mkdir(savePath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    return savePath;
}
