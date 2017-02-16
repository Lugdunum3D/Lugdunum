#pragma once

#include <lug/System/Export.hpp>
#include <lug/System/Time.hpp>

namespace lug {
namespace System {

class LUG_SYSTEM_API Clock
{
public:
    Clock();
    Clock(const Clock& clock);
    Clock(Clock&& clock);

    Clock& operator=(const Clock& clock);
    Clock& operator=(Clock&& clock);

    ~Clock() = default;

    void reset();
    Time getElapsedTime() const;

private:
    Time _startTime;
};

} // System
} // lug
