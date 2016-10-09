#include <lug/System/Win32/PathImplWin32.hpp>

std::string lug::System::priv::PathImpl::getRoot() const {
    char *homeDrive = nullptr;
    homeDrive = getEnv("HOMEDRIVE");

    if (homeDrive == nullptr) {
        return std::string("");
    }

    std::string rootString;
    rootString += homeDrive;
    return rootString;
}

std::string lug::System::priv::PathImpl::getHome() const {
    char *homeDrive = nullptr;
    homeDrive = getEnv("HOMEDRIVE");
    char *homePath = nullptr;
    homePath = getEnv("HOMEPATH");

    if (homeDrive == nullptr || homePath == nullptr) {
        return std::string("");
    }

    std::string homeString;
    homeString += homeDrive;
    homeString += homePath;
    return homeString;
}

std::string lug::System::priv::PathImpl::getCwd() const {
    TCHAR cwd[MAX_PATH_SIZE];
    if (FAILED(GetCurrentDirectory(MAX_PATH_SIZE, cwd))) {
        return std::string("");
    }
    return cwd;
}

std::string lug::System::priv::PathImpl::getSave() const {
    TCHAR appDataPath[MAX_PATH_SIZE];
    if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appDataPath))) {
        return std::string("");
    }
    return appDataPath;
}

char *lug::System::priv::PathImpl::getEnv(char * variable) const {
    char *buffer = nullptr;
    size_t bufferSize = 0;
    if (_dupenv_s(&buffer, &bufferSize, variable) != 0 || buffer == nullptr) {
        return nullptr;
    }
    return buffer;
}
