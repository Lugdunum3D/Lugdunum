#include <cstdarg>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <lug/System/Logger.hpp>

namespace lug {

/*LUG_SYSTEM_API System::Logger& getLogger() {
    static lug::System::LoggerImpl logger;
    return logger;
}*/

namespace System {

// int _vscprintf(const char* format, std::va_list args) {
//     int retval;
//     va_list args_copy;
//     va_copy(args_copy, args);
//     retval = vsnprintf(nullptr, 0, format, args_copy);
//     va_end(args_copy);
//     return retval;
// }

// Format policies
// void ConsoleFormatPolicy::format(Logger::Buffer& buffer, const Logger::Criteria& criteria) const {
//     std::stringstream ss;

//     // [TIME] [LOG_LEVEL] [SOURCE] msg
//     ss << "[" << std::setfill(' ') << std::setw(7) << Logger::getChannelName(criteria.channel) << "] ";
//     ss << "[" << std::setfill(' ') << std::setw(7) << Logger::getTypeName(criteria.type) << "] ";
//     ss << "[" << criteria.sourceInfo.filename << "] ";

//     int size = _vscprintf(criteria.format, *criteria.args) + 1;
//     buffer.resize(size);
//     std::vsnprintf(const_cast<char*>(buffer.c_str()), size, criteria.format, *criteria.args);
//     ss << buffer;
//     buffer = ss.str();
// }

} // namespace System
} // namespace lug
