template <class FilterPolicy, class FormatPolicy, class WritePolicy>
void LoggerImpl<FilterPolicy, FormatPolicy, WritePolicy>::log(Logger::Channel channel, Logger::Type type, std::size_t verbosity,
    const SourceInfo& sourceInfo, const char* format, ...) const {
    std::va_list args;
    va_start(args, format);
    Logger::Criteria criteria{ channel, type, verbosity, sourceInfo, format, &args };
    // We have assign the `va_list` like that because std::va_list is of type array and thus
    // is not assignable.

    if (_filter.filter(criteria)) {
        Buffer buffer;
        _formatter.format(buffer, criteria);
        _writer.write(buffer);
    }
    va_end(args);
}
