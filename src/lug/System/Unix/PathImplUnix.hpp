#pragma once

#include <string>

namespace lug {
namespace System {
namespace Path {
namespace priv {

std::string root();
std::string home();
std::string cwd();
std::string save(const std::string& folderName);

}
}
}
}
