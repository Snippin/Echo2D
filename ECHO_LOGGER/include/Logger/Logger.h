#pragma once

#include <cassert>
#include <source_location>
#include <string>
#include <string_view>
#include <vector>

#define ECHO_LOG(x, ...) ECHO_LOGGER::Logger::Get().Log(x, __VA_ARGS__)
#define ECHO_WARN(x, ...) ECHO_LOGGER::Logger::Get().Warn(x, __VA_ARGS__)
#define ECHO_ERROR(x, ...) ECHO_LOGGER::Logger::Get().Error(std::source_location::current(), x, __VA_ARGS__)
#define ECHO_INIT_LOGS(console, retain) ECHO_LOGGER::Logger::Get().Init(console, retain)

namespace ECHO_LOGGER
{
    struct LogEntry
    {
        enum class LogType
        {
            INFO,
            WARN,
            ERR,
            NONE
        };

        LogType type{LogType::INFO};
        std::string log{""};
    };

    // Singleton class
    class Logger
    {
    public:
        ~Logger() = default;
        // Make `Logger` non-copyable
        Logger(const Logger &) = delete;
        Logger &operator =(const Logger &) = delete;

        static Logger &Get();

        void Init(bool console_log = true, bool retain_logs = true);

        template <typename... Args>
        void Log(const std::string &message, Args&&... args);
        template <typename... Args>
        void Warn(const std::string &message, Args&&... args);
        template <typename... Args>
        void Error(std::source_location location, const std::string &message,
            Args&&... args);

    private:
        struct LogTime
        {
            std::string day;
            std::string day_number;
            std::string month;
            std::string year;
            std::string time;

            LogTime(const std::string &date);
        };

        Logger() = default;

        std::string DateTime();

        std::vector<LogEntry> log_entries;
        bool log_added{false};
        bool initialised{false};
        bool console_log{true};
        bool retain_logs{true};
    };
}

#include "Logger.inl"
