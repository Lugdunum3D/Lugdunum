#pragma once

#include <fstream>
#include <lug/System/Logger/Handler.hpp>
#include <lug/System/Exception.hpp>

namespace lug {
namespace System {

class FileHandler : public Handler {
public:
    FileHandler(const std::string& name, const filename_t& filename, bool truncate) : Handler(name) {
        _ofs.open(filename, std::ofstream::out);
        if (!_ofs.good()) {
            // TODO: add file to lug except when it handles variadic args
            LUG_EXCEPT(FileNotFoundException, "Failed to open file");
        }
        // TODO: use this parameter
        (void)truncate;
    }
    virtual ~FileHandler() {
        _ofs.close();
    }

protected:
    virtual void handle(const priv::Message& msg) {
        _ofs << msg.formatted.c_str();
    }
    virtual void flush() {
        _ofs.flush();
    }

private:
    std::ofstream _ofs;
};

} // namespace lug
} // namespace System
