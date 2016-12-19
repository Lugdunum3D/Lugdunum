template<typename T>
inline void Logger::log(Level lvl, const T& msg) {
    try {
        priv::Message logMsg(_name, lvl);
        logMsg.raw.write("{}", msg);
        handle(logMsg);
    }
    catch (const std::exception& ex) {
        defaultErrHandler(ex.what());
    }
    catch (...) {
        defaultErrHandler("Unknown exception");
    }
}

template<typename... Args, typename T>
inline void Logger::log(Level lvl, const T& fmt, Args&&... args) {
    try {
        priv::Message logMsg(_name, lvl);
        logMsg.raw.write(fmt, std::forward<Args>(args)...);
        handle(logMsg);
    }
    catch (const std::exception& ex) {
        defaultErrHandler(ex.what());
    }
    catch (...) {
        defaultErrHandler("Unknown exception");
    }
}

template<typename T, typename... Args>
inline void Logger::debug(const T& fmt, Args&&... args) {
    log(Level::Debug, fmt, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
inline void Logger::info(const T& fmt, Args&&... args) {
    log(Level::Info, fmt, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
inline void Logger::warn(const T& fmt, Args&&... args) {
    log(Level::Warning, fmt, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
inline void Logger::error(const T& fmt, Args&&... args) {
    log(Level::Error, fmt, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
inline void Logger::fatal(const T& fmt, Args&&... args) {
    log(Level::Fatal, fmt, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
inline void Logger::assrt(const T& fmt, Args&&... args) {
    log(Level::Assert, fmt, std::forward<Args>(args)...);
}


inline Logger* makeLogger(const std::string& loggerName) {
    std::unique_ptr<Logger> logger = std::make_unique<Logger>(loggerName);
    Logger* loggerRawPtr = logger.get();
    LoggingFacility::registerLogger(loggerName, std::move(logger));
    return loggerRawPtr;
}
