#pragma once

#include <lug/System/Path.hpp>

namespace lug {
namespace System {
namespace Path {
namespace priv {
std::string getRoot();
std::string getHome();
std::string getCwd();
std::string getSave(std::string folderName);
char* getEnv(char* variable);
};
};
};
};
