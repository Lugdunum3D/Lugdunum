#pragma once

#include <fstream>
#include <lug/System/Export.hpp>
#include <lug/System/Logger/Handler.hpp>
#include <lug/System/Logger/Message.hpp>

namespace lug {
namespace System {
namespace Logger {

class LUG_SYSTEM_API FileHandler : public Handler {
public:
    // TODO: handle windows's wstring for filenames, had a nasty compiler error and this can be
    // fixed later
    FileHandler(const std::string& name, const std::string& filename, bool truncate);

    FileHandler(const FileHandler&) = delete;
    FileHandler(FileHandler&&) = delete;

    FileHandler& operator=(const FileHandler&) = delete;
    FileHandler& operator=(FileHandler&&) = delete;

    ~FileHandler();

    void handle(const priv::Message& msg);
    void flush();

private:
    std::ofstream _ofs;
};

} // Logger
} // System
} // lug
