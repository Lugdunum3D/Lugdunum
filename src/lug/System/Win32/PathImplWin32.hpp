#pragma once

#include <Shlobj.h>
#include <string>
#include <windows.h>

#define MAX_PATH_SIZE 4096

namespace lug {
namespace System {
namespace Path {
    std::string getRoot();
    std::string getHome();
    std::string getCwd();
    std::string getSave(std::string folderName);
    char* getEnv(char* variable);
};
};
};
