#include <lug/System/Logger/Formatter.hpp>
#include <chrono>

namespace lug {
namespace System {
namespace priv {

std::string UserChars::format() const {
    return _chars;
}

void UserChars::addChar(char c) {
    _chars += c;
}

} // namespace priv

inline void Formatter::handleFlag(char flag) {
    switch (flag) {
    case 'v':
        _formatChain.push_back(priv::Token());
    }
}

inline void Formatter::compilePattern(const std::string& pattern)
{
    std::shared_ptr<priv::UserChars> chars;

    auto end = pattern.end();
    for (auto it = pattern.begin(); it != end; ++it) {
        if (*it == '%') {
            if (chars) { //append raw chars found so far
                _formatChain.push_back(priv::Token(chars));
                chars = nullptr;
            }

            if (++it != end)
                handleFlag(*it);
            else
                break;
        }
        else { // chars not following the % sign should be displayed as is
            if (!chars)
                chars = std::make_shared<priv::UserChars>();
            chars->addChar(*it);
        }
    }
    if (chars) { //append raw chars found so far
        _formatChain.push_back(priv::Token(chars));
    }

}

Formatter::Formatter(const std::string& pattern) {
    compilePattern(pattern);
}

void Formatter::format(priv::Message &msg) {
    for (priv::Token elem : _formatChain) {
        if (!elem.basic && !elem.advanced) {
            msg.formatted << msg.raw.str();
        }
        else if (elem.basic) {
            msg.formatted << (this->*elem.basic)();
        }
        else if (elem.advanced) {
            msg.formatted << elem.advanced->format();
        }
    }
}

} // namespace lug
} // namespace System
