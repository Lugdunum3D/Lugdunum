#include <lug/System/Win32/PathImplWin32.hpp>

#include <Shlobj.h>
#include <windows.h>

std::string lug::System::Path::priv::root() {
    char* driveLetter = getEnv("HOMEDRIVE");
    if (driveLetter == nullptr) {
        return std::string("");
    }

    std::string rootPath;
    rootPath += driveLetter;

    free(driveLetter);

    return rootPath;
}

std::string lug::System::Path::priv::home() {
    char* driveLetter = getEnv("HOMEDRIVE");
    char* homeDirectory = getEnv("HOMEPATH");

    if (driveLetter == nullptr || homeDirectory == nullptr) {
        return std::string("");
    }

    std::string homePath;
    homePath += driveLetter;
    homePath += homeDirectory;

    free(driveLetter);
    free(homeDirectory);

    return homePath;
}

std::string lug::System::Path::priv::cwd() {
    TCHAR cwd[MAX_PATH];
    if (FAILED(GetCurrentDirectory(MAX_PATH, cwd))) {
        return std::string("");
    }

    std::basic_string<TCHAR> cwdString(cwd);
    std::string cwdPath(cwdString);

    return cwdPath;
}

std::string lug::System::Path::priv::save(const std::string& folderName) {
    TCHAR appDataPath[MAX_PATH];
    if (FAILED(SHGetFolderPath(nullptr, CSIDL_APPDATA, nullptr, 0, appDataPath))) {
        return std::string("");
    }

    std::basic_string<TCHAR> appData(appDataPath);
    std::string savePath(appData);
    savePath += "\\";
    savePath += folderName;

    return savePath;
}

char* lug::System::Path::priv::getEnv(const char* variable) {
    char* buffer = nullptr;
    size_t bufferSize = 0;
    if (_dupenv_s(&buffer, &bufferSize, variable) != 0 || buffer == nullptr) {
        return nullptr;
    }
    return buffer;
}
