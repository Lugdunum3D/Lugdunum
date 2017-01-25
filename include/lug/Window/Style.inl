inline Style operator|(Style& lhs, const Style& rhs) {
    return static_cast<Style>(static_cast<uint16_t>(lhs) | static_cast<uint16_t>(rhs));
}

inline Style& operator|=(Style& lhs, const Style& rhs) {
    return (lhs = static_cast<Style>(static_cast<uint16_t>(lhs) | static_cast<uint16_t>(rhs)));
}

inline Style operator&(Style& lhs, const Style& rhs) {
    return static_cast<Style>(static_cast<uint16_t>(lhs) & static_cast<uint16_t>(rhs));
}

inline Style& operator&=(Style& lhs, const Style& rhs) {
    return (lhs = static_cast<Style>(static_cast<uint16_t>(lhs) & static_cast<uint16_t>(rhs)));
}

inline Style operator^(Style& lhs, const Style& rhs) {
    return static_cast<Style>(static_cast<uint16_t>(lhs) ^ static_cast<uint16_t>(rhs));
}

inline Style& operator^=(Style& lhs, const Style& rhs) {
    return (lhs = static_cast<Style>(static_cast<uint16_t>(lhs) ^ static_cast<uint16_t>(rhs)));
}
