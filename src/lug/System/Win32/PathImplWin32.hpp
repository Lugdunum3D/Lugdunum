#pragma once

#include <Shlobj.h>
#include <windows.h>

#define MAX_PATH_SIZE 4096

namespace lug {
namespace System {
namespace Path {
    std::string getRoot() const;
    std::string getHome() const;
    std::string getCwd() const;
    std::string getSave(std::string folderName) const;
    char* getEnv(char* variable) const;
};
};
};
