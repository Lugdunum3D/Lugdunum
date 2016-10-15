#pragma once

#include <cstdarg>
#include <string>
#include <unordered_map>
#include <lug/System/Export.hpp>
#include <lug/System/SourceInfo.hpp>


namespace lug {
namespace System {

// Generate channels list
// enum: RENDER_API, RENDER_ENGINE, PHYSICS_ENGINE, etc...
// map pair: { RENDER_API, "RENDER_API" }, { RENDER_ENGINE, "RENDER_ENGINE" }, etc...
#define LUG_LOG_CHANNELS(PROCESS)   \
    PROCESS(VULKAN),                \
    PROCESS(RENDER),                \
    PROCESS(PHYSICS),               \
    PROCESS(SCRIPT),                \
    PROCESS(NETWORK),               \
    PROCESS(SOUND),                 \
    PROCESS(MEMORY),                \
    PROCESS(WINDOW),                \
    PROCESS(MATH),                  \
    PROCESS(USER),                  \

#define LUG_LOG_TYPES(PROCESS)      \
    PROCESS(INFO),                  \
    PROCESS(WARNING),               \
    PROCESS(ERROR),                 \
    PROCESS(FATAL),                 \
    PROCESS(ASSERT),                \

#define LUG_LOG_ENUM(CHANNEL) CHANNEL
#define LUG_LOG_MAP_PAIR_C(CHANNEL) { Logger::Channel::CHANNEL, #CHANNEL }
#define LUG_LOG_MAP_PAIR_T(CHANNEL) { Logger::Type::CHANNEL, #CHANNEL }


class LUG_SYSTEM_API Logger {
public:
    using Buffer = std::string;

    enum class Channel : char {
        LUG_LOG_CHANNELS(LUG_LOG_ENUM)
    };

    enum class Type : char {
        LUG_LOG_TYPES(LUG_LOG_ENUM)
    };

    struct Criteria {
        Channel channel;
        Type type;
        std::size_t verbosity;
        const SourceInfo sourceInfo;
        const char* format;
        std::va_list* args;
    };

    virtual ~Logger(void) {}

    virtual void log(Channel channel, Type type, std::size_t verbosity,
        const SourceInfo& SourceInfo, const char* format, ...) const = 0;

    static const char* getChannelName(Channel c);
    static const char* getTypeName(Type t);

private:
    static std::unordered_map<Channel, const char*> _channelNames;
    static std::unordered_map<Type, const char*>    _typeNames;
};


template <class FilterPolicy, class FormatPolicy, class WritePolicy>
class LoggerImpl : public Logger {
public:
    LoggerImpl() = default;
    LoggerImpl(const LoggerImpl& other) = default;
    LoggerImpl(LoggerImpl&& other) = default;
    LoggerImpl& operator=(const LoggerImpl& other) = default;
    LoggerImpl& operator=(LoggerImpl&& other) = default;

    // Implemented in Logger.inl
    void log(Channel channel, Type type, std::size_t verbosity,
             const SourceInfo& sourceInfo, const char* format, ...) const;


private:
    FilterPolicy _filter;
    FormatPolicy _formatter;
    WritePolicy _writer;
};

#include <lug/System/Logger.inl>


// Filter policies
struct LUG_SYSTEM_API NoFilterPolicy {
    bool filter(const Logger::Criteria& criteria) const;
};

struct LUG_SYSTEM_API FilterPolicy {
    bool filter(const Logger::Criteria& criteria) const;
};

struct LUG_SYSTEM_API ChannelFilterPolicy {
    bool filter(const Logger::Criteria& criteria) const;
};


// Format policies
struct LUG_SYSTEM_API ConsoleFormatPolicy {
    void format(Logger::Buffer& buffer, const Logger::Criteria& criteria) const;
};

struct LUG_SYSTEM_API FileFormatPolicy {
    void format(Logger::Buffer& buffer, const Logger::Criteria& criteria) const;
};


// Writer policies
struct LUG_SYSTEM_API IdeWriterPolicy {
    void write(const Logger::Buffer& buffer) const;
};

struct LUG_SYSTEM_API ConsoleWriterPolicy {
    void write(const Logger::Buffer& buffer) const;
};

struct LUG_SYSTEM_API FileWriterPolicy {
    void write(const Logger::Buffer& buffer) const;
};


#define LUG_LOG(logger, channel, type, verbosity, format, ...)\
do {\
    logger->log(channel, type, verbosity, { __FILE__, LUG_SYSTEM_FUNCTION_NAME, __LINE__}, format, __VA_ARGS__); \
} while (0)


}
}
