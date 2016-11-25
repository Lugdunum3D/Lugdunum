#pragma once

#include <chrono>
#include <vector>
#include <memory>
#include <lug/System/Logger/Message.hpp>

namespace lug {
namespace System {

class Formatter;

using FlagHandlerPointer = std::string (Formatter::*)(const struct tm& now);

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

    std::string handleFlagy(const struct tm& now); // Year 2 digits
    std::string handleFlagY(const struct tm& now); // Year 4 digits
    std::string handleFlagm(const struct tm& now); // Month from 1 to 12
    std::string handleFlagd(const struct tm& now); // Day from 1 to 31
    std::string handleFlagH(const struct tm& now); // Hour from 0 to 24
    std::string handleFlagM(const struct tm& now); // Minute from 0 to 59
    std::string handleFlagS(const struct tm& now); // Sec from 0 to 61

    std::vector<priv::Token> _formatChain;
};


} // namespace lug
} // namespace System
