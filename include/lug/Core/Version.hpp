#pragma once

namespace lug {
namespace Core {

/**
 * @brief      Stores the version of the Application
 */
struct Version {
    uint32_t major : 10;
    uint32_t minor : 10;
    uint32_t patch : 12;

    operator uint32_t() const {
        return (major << 22) | (minor << 12) | patch;
    }

    static Version fromInt(uint32_t version);
};

#include <lug/Core/Version.inl>

} // Core
} // lug
