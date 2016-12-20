inline std::ostream& operator<<(std::ostream& ss, const VkResult& result) {
    ss << resultToStr(result);
    return ss;
}
