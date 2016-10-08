#pragma once

#include <windows.h>
#include <Shlobj.h>

#include "Path.hpp"

namespace                   lug
{
    namespace               System
    {
        class               PathWindows : Path {
        public:
            PathWindows() = default;
            ~PathWindows() = default;
            std::string     getRoot() const;
            std::string     getHome() const;
            std::string     getCwd() const;
            std::string     getSave() const;
        private:
            char            *getEnv(char *variable) const;
        };
    };
};