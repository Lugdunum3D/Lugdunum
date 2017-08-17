#pragma once

#include <cstdint>
#include <lug/System/Export.hpp>

namespace lug {
namespace System {

class LUG_SYSTEM_API Time {
public:
    Time() = default;
    Time(int64_t microseconds);
    Time(const Time&) = default;
    Time(Time&&) = default;

    Time& operator=(const Time&) = default;
    Time& operator=(Time&&) = default;

    ~Time() = default;

    int64_t getMicroseconds() const;

    template <typename T = float>
    T getMilliseconds() const;

    template <typename T = float>
    T getSeconds() const;

    static Time getCurrentTime();

private:
    int64_t _microseconds{0};
};

#include <lug/System/Time.inl>

} // System
} // lug
