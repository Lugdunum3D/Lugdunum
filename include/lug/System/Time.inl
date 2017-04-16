inline int64_t Time::getMicroseconds() const {
    return _microseconds;
}

template <typename T>
inline T Time::getMilliseconds() const {
    return static_cast<T>(_microseconds / T(1000));
}


template <typename T>
inline T Time::getSeconds() const {
    return static_cast<T>(_microseconds / T(1000000));
}
