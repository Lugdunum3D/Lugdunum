inline int64_t Time::getMicroseconds() const {
    return _microseconds;
}

inline int32_t Time::getMilliseconds() const {
    return static_cast<int32_t>(_microseconds / 1000);
}


inline float Time::getSeconds() const {
    return _microseconds / 1000000.0f;
}
