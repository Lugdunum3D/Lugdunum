#include <lug/System/Logger/Formatter.hpp>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <lug/System/Logger/Message.hpp>

namespace lug {
namespace System {
namespace Logger {
namespace priv {

std::string UserChars::format(Message*) const {
    return _chars;
}

void UserChars::addChar(char c) {
    _chars += c;
}

std::string LevelFlag::format(Message* message) const {
    if (!message) {
        return "NoMsg!";
    }

    std::stringstream ss;
    ss << std::left << std::setfill(' ') << std::setw(7) << message->level;
    std::string tmp(ss.str());
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), [](char c) {
        return static_cast<char>(toupper(c));
    });
    return tmp;
}

std::string MessageFlag::format(Message* message) const {
    if (!message) {
        return "NoMsg!";
    }

    return message->raw.str();
}

} // namespace priv

std::string Formatter::handleFlagy(const std::tm* now) {
    std::stringstream ss;
    ss << std::put_time(now, "%y");
    return ss.str();
}

std::string Formatter::handleFlagY(const std::tm* now) {
    std::stringstream ss;
    ss << std::put_time(now, "%Y");
    return ss.str();
}

std::string Formatter::handleFlagm(const std::tm* now) {
    std::stringstream ss;
    ss << std::put_time(now, "%m");
    return ss.str();
}

std::string Formatter::handleFlagd(const std::tm* now) {
    std::stringstream ss;
    ss << std::put_time(now, "%d");
    return ss.str();
}

std::string Formatter::handleFlagH(const std::tm* now) {
    std::stringstream ss;
    ss << std::put_time(now, "%H");
    return ss.str();
}

std::string Formatter::handleFlagM(const std::tm* now) {
    std::stringstream ss;
    ss << std::put_time(now, "%M");
    return ss.str();
}

std::string Formatter::handleFlagS(const std::tm* now) {
    std::stringstream ss;
    ss << std::put_time(now, "%S");
    return ss.str();
}

inline void Formatter::handleFlag(char flag) {
    priv::Token token;

    switch (flag) {
        case 'y':
            token.basic = &Formatter::handleFlagy;
            break;
        case 'Y':
            token.basic = &Formatter::handleFlagY;
            break;
        case 'm':
            token.basic = &Formatter::handleFlagm;
            break;
        case 'd':
            token.basic = &Formatter::handleFlagd;
            break;
        case 'H':
            token.basic = &Formatter::handleFlagH;
            break;
        case 'M':
            token.basic = &Formatter::handleFlagM;
            break;
        case 'S':
            token.basic = &Formatter::handleFlagS;
            break;
        case 'v':
            token.advanced = std::make_unique<priv::MessageFlag>();
            break;
        case 'l':
            token.advanced = std::make_unique<priv::LevelFlag>();
            break;
    }

    _formatChain.push_back(std::move(token));
}

inline void Formatter::compilePattern(const std::string& pattern) {
    std::unique_ptr<priv::UserChars> chars;

    auto end = pattern.end();

    for (auto it = pattern.begin(); it != end; ++it) {
        if (*it == '%') {
            if (chars) {
                // Append raw chars found so far
                _formatChain.push_back(priv::Token(std::move(chars)));
                chars = nullptr;
            }

            if (++it != end) {
                handleFlag(*it);
            } else {
                break;
            }
        } else {
            // Chars not following the % sign should be displayed as is
            if (!chars) {
                chars = std::make_unique<priv::UserChars>();
            }

            chars->addChar(*it);
        }
    }

    if (chars) {
        // Append raw chars found so far
        _formatChain.push_back(priv::Token(std::move(chars)));
    }
}

Formatter::Formatter(const std::string& pattern) {
    compilePattern(pattern);
}

Formatter::~Formatter() = default;

void Formatter::format(priv::Message& msg) {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(now);
    std::tm tf;

#if defined(LUG_SYSTEM_WINDOWS)
    // Use windows secure versions of localtime
    localtime_s(&tf, &tt);
#else
    // Use linux secure versions of localtime
    // TODO: test on android
    // It could not work (what is the localtime secure version for android ?)
    localtime_r(&tt, &tf);
#endif

    format(msg, &tf);
}

void Formatter::format(priv::Message& msg, const std::tm* now) {
    msg.formatted.clear();

    for (priv::Token& elem : _formatChain) {
        if (elem.basic) {
            msg.formatted << (this->*elem.basic)(now);
        } else if (elem.advanced) {
            msg.formatted << elem.advanced->format(&msg);
        }
    }
}

} // Logger
} // System
} // lug
