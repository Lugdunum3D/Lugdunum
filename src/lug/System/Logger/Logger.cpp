#include <lug/System/Logger.hpp>

namespace lug {
namespace System {

std::unordered_map<Logger::Channel, const char*>   Logger::_channelNames = { LUG_LOG_CHANNELS(LUG_LOG_MAP_PAIR_C) };
std::unordered_map<Logger::Type, const char*>      Logger::_typeNames = { LUG_LOG_TYPES(LUG_LOG_MAP_PAIR_T) };

const char* Logger::getChannelName(Channel c) {
    return _channelNames[c];
}

const char* Logger::getTypeName(Type t) {
    return _typeNames[t];
}

}
}
