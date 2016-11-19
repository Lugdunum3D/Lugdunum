#include <lug/System/Logger.hpp>

namespace lug {
namespace System {


void LoggerImpl::log(Logger::Channel channel, Logger::Type type, std::size_t verbosity,
    const SourceInfo& sourceInfo, const char* format, ...) const {

}

} // namespace System
} // namespace lug
