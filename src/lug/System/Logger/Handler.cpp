#include <lug/System/Logger/Handler.hpp>
#include <lug/System/Logger/Common.hpp>
#include <lug/System/Logger/Formatter.hpp>

namespace lug {
namespace System {
namespace Logger {

Handler::Handler(const std::string& name) :
    _name(name),
    _formatter(std::make_unique<Formatter>("[%H:%M:%S][%l] %v\n")),
    _levels(Level::Debug) {}

void Handler::setFormatter(std::unique_ptr<Formatter> formatter) {
    _formatter = std::move(formatter);
}

void Handler::setPattern(const std::string& formatter) {
    _formatter = std::make_unique<Formatter>(formatter);
}

void Handler::format(priv::Message& msg) {
    _formatter->format(msg);
}

void Handler::setLevels(Level levels) {
    _levels = levels;
}

Level Handler::getLevels() const {
    return _levels;
}

bool Handler::shouldLog(Level level) const {
    return ((uint8_t)level & (uint8_t)_levels);
}

} // Logger
} // System
} // lug
