#pragma once

#include <lug/System/Path.hpp>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>

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
