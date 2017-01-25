template <typename T>
inline T radians(T degrees) {
    static_assert(std::numeric_limits<T>::is_iec559, "'radians' only accept floating-point input");
    return degrees * T(0.01745329251994329576923690768489);
}

template <typename T>
inline T degrees(T radians) {
    static_assert(std::numeric_limits<T>::is_iec559, "'degrees' only accept floating-point input");
    return radians * T(57.295779513082320876798154814105);
}

template <typename T>
inline T sin(T radians) {
    return ::std::sin(radians);
}

template <typename T>
inline T cos(T radians) {
    return ::std::cos(radians);
}

template <typename T>
inline T tan(T radians) {
    return ::std::tan(radians);
}

template <typename T>
inline T asin(T radians) {
    return ::std::asin(radians);
}

template <typename T>
inline T acos(T radians) {
    return ::std::acos(radians);
}

template <typename T>
inline T atan(T radians) {
    return ::std::atan(radians);
}

template <typename T>
inline T atan2(T y, T x) {
    return ::std::atan2(y, x);
}
