#pragma once

#include <lug/System/Logger/Formatter.hpp>

namespace lug {
namespace System {

class DefaultFormatter : public Formatter {
public:
    DefaultFormatter() = default;

    void format(priv::Message &msg);

};

} // namespace System
} // namespace lug
