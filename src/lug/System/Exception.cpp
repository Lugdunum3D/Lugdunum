#include <lug/System/Exception.hpp>
#include <sstream>

lug::System::Exception::Exception(const char *typeName, const std::string &description, const char* file, const char* function, uint32_t line) noexcept
    : _typeName{typeName}, _description{description}, _file{file}, _function{function}, _line{line} {}

const std::string& lug::System::Exception::getTypeName() const {
    return _typeName;
}

const std::string& lug::System::Exception::getDescription() const {
    return _description;
}

const std::string& lug::System::Exception::getFile() const {
    return _file;
}

const std::string& lug::System::Exception::getFunction() const {
    return _function;
}

const uint32_t lug::System::Exception::getLine() const {
    return _line;
}

const char* lug::System::Exception::what() const noexcept {
    std::stringstream msg;

    msg << _typeName << ": " << _description << std::endl;
    msg << "In " << _file;
    msg << " at " << _function << " line " << _line;

    _fullDesc = msg.str();
    return _fullDesc.c_str();
}
