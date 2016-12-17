#pragma once

#include <cstdint>
#include <lug/Core/Export.hpp>

namespace lug {
namespace Core {

class LUG_CORE_API Application {
public:
    struct Version {
        uint32_t major; // 10 bits
        uint32_t minor; // 10 bits
        uint32_t patch; // 12 bits

        uint32_t createVersion() const;

        operator uint32_t() const {
            return createVersion();
        }
    };

    struct Info {
        const char* name;
        Version version;
    };

public:
    // TODO: Define the class Application
};

#include <lug/Core/Application.inl>

} // Core
} // lug
