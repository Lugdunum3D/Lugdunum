#pragma once

#include <lug/System/Export.hpp>

#include <string>

namespace lug {
namespace System {
namespace Path {

std::string root();
std::string home();
std::string cwd();
std::string save(const std::string& folderName);

}
}
}
