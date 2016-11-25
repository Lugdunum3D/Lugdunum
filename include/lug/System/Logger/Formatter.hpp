#pragma once

#include <vector>
#include <memory>
#include <lug/System/Logger/Message.hpp>

namespace lug {
namespace System {

class Formatter;

using FlagHandlerPointer = std::string (Formatter::*)();

namespace priv {

class Formattable {
public:
    virtual std::string format() const = 0;
};

class UserChars : public Formattable {
public:
    void addChar(char c);
    virtual std::string format() const;
private:
    std::string _chars;
};

struct Token {
    Token() = default;
    Token(FlagHandlerPointer flagHandle): basic(flagHandle) {}
    Token(std::shared_ptr<Formattable> formattableHandle): advanced(formattableHandle) {}

    FlagHandlerPointer basic = nullptr;
    std::shared_ptr<Formattable> advanced = nullptr;
};

} // namespace priv

class LUG_SYSTEM_API Formatter {
public:
    Formatter(const std::string& pattern);
    virtual ~Formatter () {};

    void format(priv::Message &msqg);

private:
    void handleFlag(char c);
    void compilePattern(const std::string& pattern);

    std::vector<priv::Token> _formatChain;
};


} // namespace lug
} // namespace System
