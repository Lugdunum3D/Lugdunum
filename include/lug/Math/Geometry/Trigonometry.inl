template <typename T>
inline T radians(T degrees) {
    static_assert(std::numeric_limits<T>::is_iec559, "'radians' only accept floating-point input");
    return degrees * static_cast<T>(0.01745329251994329576923690768489);
}

template <typename T>
inline T degrees(T radians) {
    static_assert(std::numeric_limits<T>::is_iec559, "'degrees' only accept floating-point input");
    return radians * static_cast<T>(57.295779513082320876798154814105);
}
