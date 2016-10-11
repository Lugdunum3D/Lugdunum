#include <lug/System/Win32/PathImplWin32.hpp>

std::string lug::System::Path::getRoot() const {
    char* driveLetter = getEnv("HOMEDRIVE");
    if (driveLetter == nullptr) {
        return std::string("");
    }
    std::string rootPath;
    rootPath += driveLetter;
    return rootPath;
}

std::string lug::System::Path::getHome() const {
    char* driveLetter =  getEnv("HOMEDRIVE");
    char* homeDirectory = getEnv("HOMEPATH");
    if (driveLetter == nullptr || homeDirectory == nullptr) {
        return std::string("");
    }
    std::string homePath;
    homePath += driveLetter;
    homePath += homeDirectory;
    return homePath;
}

std::string lug::System::Path::getCwd() const {
    TCHAR cwd[MAX_PATH_SIZE];
    if (FAILED(GetCurrentDirectory(MAX_PATH_SIZE, cwd))) {
        return std::string("");
    }
    return cwd;
}

std::string lug::System::Path::getSave(std::string folderName) const {
    TCHAR appDataPath[MAX_PATH_SIZE];
    if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appDataPath))) {
        return std::string("");
    }
    std::wstring appData(&appDataPath[0]);
    std::string savePath(appData.begin(), appData.end());
    savePath += "\\";
    savePath += folderName;
    if (!CreateDirectory((TCHAR*)plop.c_str(), NULL)) {
        return std::string("");
    }
    return appDataPath;
}

char* lug::System::Path::getEnv(char * variable) const {
    char* buffer = nullptr;
    size_t bufferSize = 0;
    if (_dupenv_s(&buffer, &bufferSize, variable) != 0 || buffer == nullptr) {
        return nullptr;
    }
    return buffer;
}
