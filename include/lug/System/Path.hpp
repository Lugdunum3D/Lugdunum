#pragma once

#include <lug/System/Export.hpp>

#include <string>
#include <iostream>

#define MAX_PATH_SIZE 4096

namespace lug {
namespace System {
namespace Path {
std::string getRoot();
std::string getHome();
std::string getCwd();
std::string getSave(std::string folderName);
};
};
};
