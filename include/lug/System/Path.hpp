#pragma once

#include <string>
#include <iostream>

#define MAX_PATH_SIZE 4096

namespace                           lug
{
    namespace                       System
    {
        class                       Path {
        public:
            virtual std::string     getRoot() const = 0;
            virtual std::string     getHome() const = 0;
            virtual std::string     getCwd() const = 0;
            virtual std::string     getSave() const = 0;
        };
    }
}