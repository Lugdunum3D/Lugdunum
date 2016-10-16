#pragma once

#include <cstdarg>
#include <string>
#include <unordered_map>
#include <lug/System/Export.hpp>
#include <lug/System/SourceInfo.hpp>
#include <lug/System/Utils.hpp>


namespace lug {
namespace System {

// Generate channels list
// enum: RENDER_API, RENDER_ENGINE, PHYSICS_ENGINE, etc...
// map pair: { RENDER_API, "RENDER_API" }, { RENDER_ENGINE, "RENDER_ENGINE" }, etc...
#define LUG_LOG_CHANNELS(PROCESS)   \
    PROCESS(Vulkan),                \
    PROCESS(Render),                \
    PROCESS(Physics),               \
    PROCESS(Script),                \
    PROCESS(Network),               \
    PROCESS(Sound),                 \
    PROCESS(Memory),                \
    PROCESS(Window),                \
    PROCESS(Math),                  \
    PROCESS(User),                  \

#define LUG_LOG_TYPES(PROCESS)      \
    PROCESS(Info),                  \
    PROCESS(Warning),               \
    PROCESS(Error),                 \
    PROCESS(Fatal),                 \
    PROCESS(Assert),                \

#define LUG_LOG_ENUM(CHANNEL) CHANNEL
#define LUG_LOG_MAP_PAIR_C(CHANNEL) { Logger::Channel::CHANNEL, #CHANNEL }
#define LUG_LOG_MAP_PAIR_T(CHANNEL) { Logger::Type::CHANNEL, #CHANNEL }


class LUG_SYSTEM_API Logger {
public:
    using Buffer = std::string;

    enum class Channel : uint8_t {
        LUG_LOG_CHANNELS(LUG_LOG_ENUM)
    };

    enum class Type : uint8_t {
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

    static std::string getChannelName(Channel c);
    static std::string getTypeName(Type t);

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


#define LUG_LOG(logger, channel, type, verbosity, ...)\
do {\
    logger->log(channel, type, verbosity, { __FILE__, LUG_SYSTEM_FUNCTION_NAME, __LINE__}, __VA_ARGS__); \
} while (0)

// Define each type as its own macro to facilitate calling
#define LUG_LOG_INFO(logger, channel, verbosity, ...)\
    LUG_LOG(logger, channel, lug::System::Logger::Type::Info, verbosity, __VA_ARGS__)

#define LUG_LOG_WARNING(logger, channel, verbosity, ...)\
    LUG_LOG(logger, channel, lug::System::Logger::Type::Warning, verbosity, __VA_ARGS__)

#define LUG_LOG_ERROR(logger, channel, verbosity, ...)\
    LUG_LOG(logger, channel, lug::System::Logger::Type::Error, verbosity, __VA_ARGS__)

#define LUG_LOG_FATAL(logger, channel, verbosity, ...)\
    LUG_LOG(logger, channel, lug::System::Logger::Type::Fatal, verbosity, __VA_ARGS__)

#define LUG_LOG_ASSERT(logger, channel, verbosity, ...)\
    LUG_LOG(logger, channel, lug::System::Logger::Type::Assert, verbosity, __VA_ARGS__)

} // namespace System

#if !defined(LUG_NO_SHORT_LOG)

    lug::System::Logger &getLogger();

    #define LOGINF(...)\
    do {\
        lug::System::Logger *logger = &lug::getLogger(); \
        LUG_LOG(logger, lug::System::Logger::Channel::User, lug::System::Logger::Type::Info, 1, __VA_ARGS__); \
    } while (0)

    #define LOGWRN(...)\
    do {\
        lug::System::Logger *logger = &lug::getLogger(); \
        LUG_LOG(logger, lug::System::Logger::Channel::User, lug::System::Logger::Type::Warning, 1, __VA_ARGS__); \
    } while (0)

    #define LOGERR(...)\
    do {\
        lug::System::Logger *logger = &lug::getLogger(); \
        LUG_LOG(logger, lug::System::Logger::Channel::User, lug::System::Logger::Type::Error, 1, __VA_ARGS__); \
    } while (0)

    #define LOGFAT(...)\
    do {\
        lug::System::Logger *logger = &lug::getLogger(); \
        LUG_LOG(logger, lug::System::Logger::Channel::User, lug::System::Logger::Type::Fatal, 1, __VA_ARGS__); \
    } while (0)

    #define LOGASR(...)\
    do {\
        lug::System::Logger *logger = &lug::getLogger(); \
        LUG_LOG(logger, lug::System::Logger::Channel::User, lug::System::Logger::Type::Assert, 1, __VA_ARGS__); \
    } while (0)

#endif

} // namespace lug

