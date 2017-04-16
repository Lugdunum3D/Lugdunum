#include <chrono>
#include <lug/System/Clock.hpp>

namespace lug {
namespace System {

Time::Time(const Time& time) : _microseconds(time._microseconds) {}

Time::Time(int64_t microseconds) : _microseconds(microseconds) {}

Time Time::getCurrentTime() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    int64_t microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
    return Time(microseconds);
}

} // System
} // lug
