#pragma once

#include <fstream>
#include <lug/System/Exception.hpp>
#include <lug/System/Logger/Handler.hpp>
#include <lug/System/Logger/Message.hpp>

namespace lug {
namespace System {

class LUG_SYSTEM_API FileHandler : public Handler {
public:
    // TODO: handle windows's wstring for filenames, had a nasty compiler error and this can be
    // fixed later
    FileHandler(const std::string& name, const std::string& filename, bool truncate) : Handler(name) {
        std::ios_base::openmode mode = std::ofstream::out;
        if (truncate)
            mode |= std::ofstream::trunc;
        _ofs.open(filename, mode);
        if (!_ofs.good()) {
            // TODO: add file to lug except when it handles variadic args
            LUG_EXCEPT(FileNotFoundException, "Failed to open file");
        }
    }
    ~FileHandler() {
        _ofs.close();
    }

    void handle(const priv::Message& msg) {
        _ofs << msg.formatted.c_str();
    }
    void flush() {
        _ofs.flush();
    }

private:
    std::ofstream _ofs;
};

} // namespace lug
} // namespace System
