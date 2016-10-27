#include <cstdarg>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <lug/System/Logger.hpp>

namespace lug {

LUG_SYSTEM_API System::Logger& getLogger() {
    static lug::System::LoggerImpl<lug::System::NoFilterPolicy, lug::System::ConsoleFormatPolicy, lug::System::ConsoleWriterPolicy> logger;
    return logger;
}

namespace System {


// Filter policies
bool NoFilterPolicy::filter(const Logger::Criteria& /*criteria*/) const {
    return true;
}

bool FilterPolicy::filter(const Logger::Criteria& /*criteria*/) const {
    return true;
}

bool ChannelFilterPolicy::filter(const Logger::Criteria& /*criteria*/) const {
    return true;
}


int _vscprintf(const char* format, std::va_list args) {
    int retval;
    va_list args_copy;
    va_copy(args_copy, args);
    retval = vsnprintf(nullptr, 0, format, args_copy);
    va_end(args_copy);
    return retval;
}

// Format policies
void ConsoleFormatPolicy::format(Logger::Buffer& buffer, const Logger::Criteria& criteria) const {
    std::stringstream ss;

    // [TIME] [LOG_LEVEL] [SOURCE] msg
    ss << "[" << std::setfill(' ') << std::setw(7) << Logger::getChannelName(criteria.channel) << "] ";
    ss << "[" << std::setfill(' ') << std::setw(7) << Logger::getTypeName(criteria.type) << "] ";
    ss << "[" << criteria.sourceInfo.filename << "] ";

    int size = _vscprintf(criteria.format, *criteria.args) + 1;
    buffer.resize(size);
    std::vsnprintf(const_cast<char*>(buffer.c_str()), size, criteria.format, *criteria.args);
    ss << buffer;
    buffer = ss.str();
}

void FileFormatPolicy::format(Logger::Buffer& /*buffer*/, const Logger::Criteria& /*criteria*/) const {}


// Writer policies
void IdeWriterPolicy::write(const Logger::Buffer& /*buffer*/) const {}

void ConsoleWriterPolicy::write(const Logger::Buffer& buffer) const {
    std::cout << buffer << std::endl;
}

void FileWriterPolicy::write(const Logger::Buffer& /*buffer*/) const {}

} // namespace System
} // namespace lug
