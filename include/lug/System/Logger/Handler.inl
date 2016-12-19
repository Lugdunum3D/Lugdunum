template<typename T, typename... Args>
inline T* makeHandler(const std::string& handlerName, Args&... args) {
    std::unique_ptr<T> handler = std::make_unique<T>(handlerName, args...);
    T* handlerRawPtr = handler.get();
    LoggingFacility::registerHandler(handlerName, std::move(handler));
    return handlerRawPtr;
}
