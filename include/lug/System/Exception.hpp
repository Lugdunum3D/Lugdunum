#pragma once

#include <exception>
#include <cstdint>
#include <string>
#include <lug/System/Export.hpp>


namespace lug {
namespace System {

class LUG_SYSTEM_API Exception: public std::exception  {
public:
    Exception(const char *typeName, const std::string &description, const char* file, const char* function, uint32_t line) noexcept;
    Exception(Exception&& other) = default;
    Exception(const Exception& other) = default;
    Exception& operator=(const Exception& other) = default;
    Exception& operator=(Exception&& other) = default;
    virtual ~Exception() throw() {}

    const std::string& getTypeName() const;
    const std::string& getDescription() const;
    const std::string& getFile() const;
    const std::string& getFunction() const;
    const uint32_t getLine() const;

    const char* what() const noexcept override;

private:
    std::string _typeName;
    std::string _description;
    std::string _file;
    std::string _function;
    uint32_t _line;

    mutable std::string _fullDesc;
};


class LUG_SYSTEM_API NotImplementedException: public Exception {
public:
    NotImplementedException(const std::string &description, const char* file, const char* function, uint32_t line)
    : Exception{"NotImplementedException", description, file, function, line} {}
};


class LUG_SYSTEM_API FileNotFoundException: public Exception {
public:
    FileNotFoundException(const std::string &description, const char* file, const char* function, uint32_t line)
    : Exception{"FileNotFoundException", description, file, function, line} {}
};


class LUG_SYSTEM_API IOException: public Exception {
public:
    IOException(const std::string &description, const char* file, const char* function, uint32_t line)
    : Exception{"IOException", description, file, function, line} {}
};


class LUG_SYSTEM_API InvalidParametersException: public Exception {
public:
    InvalidParametersException(const std::string &description, const char* file, const char* function, uint32_t line)
    : Exception{"InvalidParametersException", description, file, function, line} {}
};


class LUG_SYSTEM_API InternalErrorException: public Exception {
public:
    InternalErrorException(const std::string &description, const char* file, const char* function, uint32_t line)
    : Exception{"InternalErrorException", description, file, function, line} {}
};


class LUG_SYSTEM_API RendererException: public Exception {
public:
    RendererException(const std::string &description, const char* file, const char* function, uint32_t line)
    : Exception{"RendererException", description, file, function, line} {}
};


class LUG_SYSTEM_API WindowException: public Exception {
public:
    WindowException(const std::string &description, const char* file, const char* function, uint32_t line)
    : Exception{"WindowException", description, file, function, line} {}
};


#if defined(LUG_COMPILER_MSVC)
    #define LUG_SYSTEM_FUNCTION_NAME __FUNCSIG__
#else
    #define LUG_SYSTEM_FUNCTION_NAME __PRETTY_FUNCTION__
#endif

#define LUG_EXCEPT(type, desc)\
do {\
    throw lug::System::type(desc, __FILE__, LUG_SYSTEM_FUNCTION_NAME, __LINE__);\
} while (0)

}
}
