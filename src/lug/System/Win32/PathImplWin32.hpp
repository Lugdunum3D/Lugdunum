#pragma once

#include <lug/System/Path.hpp>
#include <windows.h>
#include <Shlobj.h>

namespace lug {
namespace System {
namespace priv {
class PathImpl {
public:
    PathImpl() = default;
    ~PathImpl() = default;
    PathImpl(PathImpl const &) = default;
    PathImpl &operator=(const PathImpl &) = default;
    std::string getRoot() const;
    std::string getHome() const;
    std::string getCwd() const;
    std::string getSave() const;
private:
    char *getEnv(char *variable) const;
};
};
};
};
