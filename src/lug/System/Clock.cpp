#include <lug/System/Clock.hpp>

namespace lug {
namespace System {

Clock::Clock() : _startTime(Time::getCurrentTime()) {}

Clock::Clock(const Clock& clock)
{
    _startTime = clock._startTime;
}

Clock::Clock(Clock&& clock)
{
    _startTime = clock._startTime;
}


Clock& Clock::operator=(const Clock& clock)
{
    _startTime = clock._startTime;
    return *this;
}

Clock& Clock::operator=(Clock&& clock)
{
    _startTime = clock._startTime;
    return *this;
}

Time Clock::getElapsedTime() const {
    return Time(Time::getCurrentTime().getMicroseconds() - _startTime.getMicroseconds());
}

void Clock::reset() {
    _startTime = Time::getCurrentTime();
}

} // System
} // lug
