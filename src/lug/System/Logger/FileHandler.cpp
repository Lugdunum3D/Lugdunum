#include <lug/System/Logger/FileHandler.hpp>
#include <lug/System/Exception.hpp>

namespace lug {
namespace System {

FileHandler::FileHandler(const std::string& name, const std::string& filename, bool truncate) : Handler(name) {
    std::ios_base::openmode mode = std::ofstream::out;

    if (truncate) {
        mode |= std::ofstream::trunc;
    }

    _ofs.open(filename, mode);

    if (!_ofs.good()) {
        // TODO: add file to lug except when it handles variadic args
        LUG_EXCEPT(FileNotFoundException, "Failed to open file");
    }
}

FileHandler::~FileHandler() {
    _ofs.close();
}

void FileHandler::handle(const priv::Message& msg) {
    _ofs << msg.formatted.c_str();
}

void FileHandler::flush() {
    _ofs.flush();
}

} // namespace lug
} // namespace System
