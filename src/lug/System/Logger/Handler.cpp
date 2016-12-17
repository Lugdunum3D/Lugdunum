#include <lug/System/Logger/Handler.hpp>

namespace lug {
namespace System {

Handler::Handler(const std::string& name):
    _name(name),
    _formatter(std::make_unique<Formatter>("%v\n")),
    _level(Level::Debug)
{}

Handler::~Handler() {}

void Handler::setFormatter(std::unique_ptr<Formatter> formatter) {
    _formatter = std::move(formatter);
}

void Handler::format(priv::Message& msg) {
    _formatter->format(msg);
}

void Handler::setLevel(Level::enumLevel level) {
    _level.store(level);
}

Level::enumLevel Handler::getLevel() const {
    return static_cast<Level::enumLevel>(_level.load(std::memory_order_relaxed));
}

bool Handler::shouldLog(Level::enumLevel level) const {
    return level >= _level.load(std::memory_order_relaxed);
}

} // namespace System
} // namespace lug
