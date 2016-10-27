#include <lug/System/Logger.hpp>

namespace lug {
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


// Format policies
void ConsoleFormatPolicy::format(Logger::Buffer& /*buffer*/, const Logger::Criteria& /*criteria*/) const {}

void FileFormatPolicy::format(Logger::Buffer& /*buffer*/, const Logger::Criteria& /*criteria*/) const {}


// Writer policies
void IdeWriterPolicy::write(const Logger::Buffer& /*buffer*/) const {}

void ConsoleWriterPolicy::write(const Logger::Buffer& /*buffer*/) const {}

void FileWriterPolicy::write(const Logger::Buffer& /*buffer*/) const {}

} // namespace System
} // namespace lug
