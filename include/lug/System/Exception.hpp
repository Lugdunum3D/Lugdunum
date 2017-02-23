#pragma once

#include <exception>
#include <cstdint>
#include <string>
#include <lug/System/Export.hpp>
#include <lug/System/Utils.hpp>

namespace lug {
namespace System {

class LUG_SYSTEM_API Exception: public std::exception {
public:
    /**
     * \brief Exception constructor.
     * \param typeName The exception type.
     * \param description The exception description.
     * \param file The file where exception was thrown.
     * \param function The function where exception was thrown.
     * \param line The line where exception was thrown.
     */
    Exception(const char *typeName, const std::string &description, const char* file, const char* function, uint32_t line);
    Exception(Exception&& other) = default;
    Exception(const Exception& other) = default;
    Exception& operator=(const Exception& other) = default;
    Exception& operator=(Exception&& other) = default;
    virtual ~Exception() throw() {}

    /**
     * \brief TypeName getter.
     * \return The typeName of the current exception.
     */
    const std::string& getTypeName() const;

    /**
     * \brief Description getter.
     * \return The description of the current exception.
     */
    const std::string& getDescription() const;

    /**
     * \brief File getter.
     * \return The file of the current exception.
     */
    const std::string& getFile() const;

    /**
     * \brief Function getter.
     * \return The function of the current exception.
     */
    const std::string& getFunction() const;

    /**
     * \brief Line getter.
     * \return The line of the current exception.
     */
    uint32_t getLine() const;

    /**
     * \brief Return the full formated exception.
     * \return The full formated exceptionn.
     */
    const char* what() const noexcept override;

private:
    std::string _typeName;
    std::string _description;
    std::string _file;
    std::string _function;
    uint32_t _line;

    mutable std::string _fullDesc;
};

class LUG_SYSTEM_API RTTIException: public Exception {
public:
    RTTIException(const std::string &description, const char* file, const char* function, uint32_t line)
    : Exception{"RTTIException", description, file, function, line} {}
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


#define LUG_EXCEPT(type, desc)\
do {\
    throw ::lug::System::type(desc, __FILE__, LUG_SYSTEM_FUNCTION_NAME, __LINE__);\
} while (0)

} // System
} // lug
