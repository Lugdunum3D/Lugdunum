#include <lug/System/Win32/PathImplWin32.hpp>

#include <Shlobj.h>
#include <utility>
#include <windows.h>

std::string lug::System::Path::priv::root() {
    char* driveLetter = getEnv("HOMEDRIVE");
    if (driveLetter == nullptr) {
        return std::string("");
    }

    const std::string rootPath(driveLetter);

    free(driveLetter);

    return std::move(rootPath);
}

std::string lug::System::Path::priv::home() {
    char* driveLetter = getEnv("HOMEDRIVE");
    char* homeDirectory = getEnv("HOMEPATH");

    if (driveLetter == nullptr || homeDirectory == nullptr) {
        free(driveLetter);
        free(homeDirectory);
        return std::string("");
    }

    std::string homePath(driveLetter);
    homePath += homeDirectory;

    free(driveLetter);
    free(homeDirectory);

    return std::move(homePath);
}

std::string lug::System::Path::priv::cwd() {
    TCHAR cwd[MAX_PATH];
    if (FAILED(GetCurrentDirectory(MAX_PATH, cwd))) {
        return std::string("");
    }

    std::wstring cwdString(&cwd[0]);
    std::string cwdPath(cwdString.begin(), cwdString.end());

    return std::move(cwdPath);
}

std::string lug::System::Path::priv::save(const std::string& folderName) {
    TCHAR appDataPath[MAX_PATH];
    if (FAILED(SHGetFolderPath(nullptr, CSIDL_APPDATA, nullptr, 0, appDataPath))) {
        return std::string("");
    }

    std::wstring appData(&appDataPath[0]);
    std::string savePath(appData.begin(), appData.end());
    savePath += "\\";
    savePath += folderName;

    return std::move(savePath);
}

char* lug::System::Path::priv::getEnv(const char* variable) {
    char* buffer = nullptr;
    size_t bufferSize = 0;
    if (_dupenv_s(&buffer, &bufferSize, variable) != 0 || buffer == nullptr) {
        return nullptr;
    }
    return buffer;
}
