#include <lug/System/Win32/PathImplWin32.hpp>

std::string lug::System::Path::getRoot() {
    char* driveLetter = getEnv("HOMEDRIVE");
    if (driveLetter == nullptr) {
        return std::string("");
    }
    std::string rootPath;
    rootPath += driveLetter;
    return rootPath;
}

std::string lug::System::Path::getHome() {
    char* driveLetter = getEnv("HOMEDRIVE");
    char* homeDirectory = getEnv("HOMEPATH");
    if (driveLetter == nullptr || homeDirectory == nullptr) {
        return std::string("");
    }
    std::string homePath;
    homePath += driveLetter;
    homePath += homeDirectory;
    return homePath;
}

std::string lug::System::Path::getCwd() {
    TCHAR cwd[MAX_PATH_SIZE];
    if (FAILED(GetCurrentDirectory(MAX_PATH_SIZE, cwd))) {
        return std::string("");
    }
    std::wstring cwdString(&cwd[0]);
    std::string cwdPath(cwdString.begin(), cwdString.end());
    return cwdPath;
}

std::string lug::System::Path::getSave(std::string folderName) {
    TCHAR appDataPath[MAX_PATH_SIZE];
    if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appDataPath))) {
        return std::string("");
    }
    std::wstring appData(&appDataPath[0]);
    std::string savePath(appData.begin(), appData.end());
    savePath += "\\";
    savePath += folderName;
    if (!CreateDirectory((TCHAR*)savePath.c_str(), NULL)) {
        return std::string("");
    }
    return savePath;
}

char* lug::System::Path::getEnv(char * variable) {
    char* buffer = nullptr;
    size_t bufferSize = 0;
    if (_dupenv_s(&buffer, &bufferSize, variable) != 0 || buffer == nullptr) {
        return nullptr;
    }
    return buffer;
}
