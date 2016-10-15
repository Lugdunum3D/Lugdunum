#pragma once

#include <string>

struct SourceInfo {
    std::string filename;
    std::string function;
    std::size_t line;
};
