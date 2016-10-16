#include <lug/System/Logger.hpp>
#include <algorithm>

namespace lug {
namespace System {

std::unordered_map<Logger::Channel, const char*>   Logger::_channelNames = { LUG_LOG_CHANNELS(LUG_LOG_MAP_PAIR_C) };
std::unordered_map<Logger::Type, const char*>      Logger::_typeNames = { LUG_LOG_TYPES(LUG_LOG_MAP_PAIR_T) };

std::string Logger::getChannelName(Channel c) {
    std::string str(_channelNames[c]);
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    return str;
}

std::string Logger::getTypeName(Type t) {
    std::string str(_typeNames[t]);
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    return str;
}

}
}
