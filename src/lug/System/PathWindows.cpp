#include "../../../include/lug/System/PathWindows.hpp"

std::string     lug::System::PathWindows::getRoot() const {
    char        *envBuffer = nullptr;
    std::string rootPath("");
    
    envBuffer = getEnv("HOMEDRIVE");
    rootPath += envBuffer;
    free(envBuffer);
    return (rootPath);
}

std::string     lug::System::PathWindows::getHome() const {
    char        *envBuffer = nullptr;
    std::string homePath("");


    envBuffer = getEnv("HOMEDRIVE");
    homePath += envBuffer;
    envBuffer = getEnv("HOMEPATH");
    homePath += envBuffer;
    return (homePath);
}

std::string     lug::System::PathWindows::getCwd() const {
    TCHAR       cwd[MAX_PATH_SIZE];
    
    if (FAILED(GetCurrentDirectory(MAX_PATH_SIZE, cwd))) {
        // TO DO: throw err
    }
    return (cwd);
}

std::string     lug::System::PathWindows::getSave() const
{
    TCHAR       appDataPath[MAX_PATH_SIZE];

    if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appDataPath))) {
        // TO DO: throw err
    }
    return (appDataPath);
}

char            *lug::System::PathWindows::getEnv(char * variable) const {
    char        *buffer = nullptr;
    size_t      bufferSize = 0;

    if (_dupenv_s(&buffer, &bufferSize, variable) != 0 || buffer == nullptr) {
        // TO DO: throw err
        return (nullptr);
    }
    return (buffer);
}
