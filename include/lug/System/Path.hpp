#pragma once

#include <string>
#include <iostream>
#include <lug/System/Export.hpp>

#define MAX_PATH_SIZE 4096

namespace lug {
namespace System {

namespace priv {
    class PathImpl;
} // namespace priv

class LUG_PATH_API Path
{
public:
    ~Path();
    std::string getRoot() const;
    std::string getHome() const;
    std::string getCwd() const;
    std::string getSave() const;
private:
    Path();
    priv::PathImpl *_impl;
};

} // namespace System
} // namespace lug
