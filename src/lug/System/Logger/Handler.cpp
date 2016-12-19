#include <lug/System/Logger/Handler.hpp>
#include <lug/System/Logger/Common.hpp>
#include <lug/System/Logger/Formatter.hpp>

namespace lug {
namespace System {

Handler::Handler(const std::string& name):
    _name(name),
    _formatter(std::make_unique<Formatter>("%v\n")),
    _level(static_cast<int>(Level::Debug))
{}

Handler::~Handler() {}

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
    _level.store(static_cast<int>(level));
}

Level Handler::getLevel() const {
    return static_cast<Level>(_level.load(std::memory_order_relaxed));
}

bool Handler::shouldLog(Level level) const {
    return static_cast<int>(level) >= _level.load(std::memory_order_relaxed);
}

} // namespace System
} // namespace lug
