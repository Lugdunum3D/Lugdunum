#pragma once

#include <cstdint>
#include <lug/System/Export.hpp>

namespace lug {
namespace System {

class LUG_SYSTEM_API Time
{
public:
    Time() = default;
    ~Time() = default;
    Time(const Time& time);
    Time(int64_t microseconds);

    int64_t getMicroseconds() const;
    int32_t getMilliseconds() const;
    float getSeconds() const;

    static Time getCurrentTime();

private:
    int64_t _microseconds{0};
};

#include <lug/System/Time.inl>

} // System
} // lug
