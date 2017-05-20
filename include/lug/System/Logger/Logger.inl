template<typename T>
inline void Logger::log(std::string source, Level lvl, const T& msg) {
    try {
        if (shouldLog(source, lvl)) {
            priv::Message logMsg(_name, lvl);
            logMsg.raw.write(source + ": {}", msg);
            handle(logMsg);
        }
    } catch (const std::exception& ex) {
        defaultErrHandler(ex);
    }
}

template<typename... Args, typename T>
inline void Logger::log(std::string source, Level lvl, const T& fmt, Args&&... args) {
    try {
        if (shouldLog(source, lvl)) {
            priv::Message logMsg(_name, lvl);
            std::string wrt = fmt::format(source + ": " + fmt::format(fmt, std::forward<Args>(args)...));
            logMsg.raw.write(wrt);
            handle(logMsg);
        }
    } catch (const std::exception& ex) {
        defaultErrHandler(ex);
    }
}

template<typename T, typename... Args>
inline void Logger::debug(std::string source, const T& fmt, Args&&... args) {
    log(source, Level::Debug, fmt, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
inline void Logger::info(std::string source, const T& fmt, Args&&... args) {
    log(source, Level::Info, fmt, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
inline void Logger::warn(std::string source, const T& fmt, Args&&... args) {
    log(source, Level::Warning, fmt, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
inline void Logger::error(std::string source, const T& fmt, Args&&... args) {
    log(source, Level::Error, fmt, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
inline void Logger::fatal(std::string source, const T& fmt, Args&&... args) {
    log(source, Level::Fatal, fmt, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
inline void Logger::assrt(std::string source, const T& fmt, Args&&... args) {
    log(source, Level::Assert, fmt, std::forward<Args>(args)...);
}

inline bool Logger::shouldLog(std::string source, Level level) {
    #if LOG_MODE == 0
    if (std::find(this->_srcLevels[source].begin(), this->_srcLevels[source].end(), Level::Off) != this->_srcLevels[source].end()) {
        return false;
    }
    if (std::find(this->_srcLevels[source].begin(), this->_srcLevels[source].end(), level) != this->_srcLevels[source].end()) {
        return false;
    }
    return true;
    #else
    if (_srcLevel[source] >= level) {
        return true;
    }
    return false;
    #endif
}



inline Logger* makeLogger(const std::string& loggerName) {
    std::unique_ptr<Logger> logger = std::make_unique<Logger>(loggerName);
    Logger* loggerRawPtr = logger.get();
    LoggingFacility::registerLogger(loggerName, std::move(logger));
    return loggerRawPtr;
}
