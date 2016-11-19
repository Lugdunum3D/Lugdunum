#include <lug/System/Logger/DefaultFormatter.hpp>

namespace lug {
namespace System {

void DefaultFormatter::format(priv::Message &msg) {
    msg.formatted << msg.raw.str();
}

} // namespace System
} // namespace lug
