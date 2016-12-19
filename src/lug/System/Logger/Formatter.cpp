#include <lug/System/Logger/Formatter.hpp>
#include <lug/System/Logger/Message.hpp>
#include <sstream>

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


std::string Formatter::handleFlagy(const struct tm& now) {
    std::stringstream ss;
    // tm_year is the number of years since 1900
    ss << now.tm_year + 1900;
    return ss.str().substr(2, 2);
}

std::string Formatter::handleFlagY(const struct tm& now) {
    std::stringstream ss;
    // tm_year is the number of years since 1900
    ss << now.tm_year + 1900;
    return ss.str();
}

std::string Formatter::handleFlagm(const struct tm& now) {
    std::stringstream ss;
    // tm_mon is the month of year from 0 to 11
    ss << now.tm_mon + 1;
    return ss.str();
}

std::string Formatter::handleFlagd(const struct tm& now) {
    std::stringstream ss;
    ss << now.tm_mday;
    return ss.str();
}

std::string Formatter::handleFlagH(const struct tm& now) {
    std::stringstream ss;
    ss << now.tm_hour;
    return ss.str();
}

std::string Formatter::handleFlagM(const struct tm& now) {
    std::stringstream ss;
    ss << now.tm_min;
    return ss.str();
}

std::string Formatter::handleFlagS(const struct tm& now) {
    std::stringstream ss;
    ss << now.tm_sec;
    return ss.str();
}

inline void Formatter::handleFlag(char flag) {
    switch (flag) {
    case 'v':
        _formatChain.push_back(priv::Token());
    break;
    case 'y':
        _formatChain.push_back(priv::Token(&Formatter::handleFlagy));
    break;
    case 'Y':
        _formatChain.push_back(priv::Token(&Formatter::handleFlagY));
    break;
    case 'm':
        _formatChain.push_back(priv::Token(&Formatter::handleFlagm));
    break;
    case 'd':
        _formatChain.push_back(priv::Token(&Formatter::handleFlagd));
    break;
    case 'H':
        _formatChain.push_back(priv::Token(&Formatter::handleFlagH));
    break;
    case 'M':
        _formatChain.push_back(priv::Token(&Formatter::handleFlagM));
    break;
    case 'S':
        _formatChain.push_back(priv::Token(&Formatter::handleFlagS));
    break;
    }
}

inline void Formatter::compilePattern(const std::string& pattern) {
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

void Formatter::format(priv::Message& msg) {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(now);
    struct tm timeInfo;

    msg.formatted.clear();

    #if defined(LUG_SYSTEM_WINDOWS)
    // Use windows secure versions of localtime
        localtime_s(&timeInfo, &tt);
    #else
    // Use linux secure versions of localtime
    // TODO: test on android
    // It could not work (what is the localtime secure version for android ?)
        localtime_r(&tt, &timeInfo);
    #endif

    for (priv::Token elem : _formatChain) {
        if (!elem.basic && !elem.advanced) {
            msg.formatted << msg.raw.str();
        }
        else if (elem.basic) {
            msg.formatted << (this->*elem.basic)(timeInfo);
        }
        else if (elem.advanced) {
            msg.formatted << elem.advanced->format();
        }
    }
}

} // namespace lug
} // namespace System
