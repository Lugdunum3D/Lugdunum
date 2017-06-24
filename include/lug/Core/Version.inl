inline Version Version::fromInt(uint32_t version) {
    return {
        (version >> 22) & 0x3FF, // major
        (version >> 12) & 0x3FF, // minor
        version & 0xFFF // patch
    };
}
