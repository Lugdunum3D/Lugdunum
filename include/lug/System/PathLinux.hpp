#pragma once

#include "Path.hpp"

namespace                           lug
{
    namespace                       System
    {
        class               PathLinux : Path {
        public:
            PathLinux() = default;
            ~PathLinux() = default;
            std::string     getRoot() const;
            std::string     getHome() const;
            std::string     getCwd() const;
            std::string     getSave() const;
        };
    };
};