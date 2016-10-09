#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>

#include "Path.hpp"

namespace               lug
{
namespace               System
{
    class               PathLinux : Path {
    public:
        PathLinux() = default;
        ~PathLinux() = default;
        PathLinux(PathLinux const &) = default;
        PathLinux &operator=(const PathLinux &) = default;
        std::string     getRoot() const;
        std::string     getHome() const;
        std::string     getCwd() const;
        std::string     getSave() const;
    };
};
};