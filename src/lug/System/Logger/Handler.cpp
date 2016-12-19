#include <lug/System/Logger/Handler.hpp>
#include <lug/System/Logger/Common.hpp>
#include <lug/System/Logger/Formatter.hpp>

namespace lug {
namespace System {

Handler::Handler(const std::string& name):
    _name(name),
    _formatter(std::make_unique<Formatter>("%v\n")),
    _level(Level::Debug) {}

void Handler::setFormatter(std::unique_ptr<Formatter> formatter) {
    _formatter = std::move(formatter);
}

void Handler::setPattern(const std::string& formatter) {
    _formatter = std::make_unique<Formatter>(formatter);
}

void Handler::format(priv::Message& msg) {
    _formatter->format(msg);
}

void Handler::setLevel(Level level) {
    _level = level;
}

Level Handler::getLevel() const {
    return _level;
}

bool Handler::shouldLog(Level level) const {
    return level >= _level;
}

} // namespace System
} // namespace lug
