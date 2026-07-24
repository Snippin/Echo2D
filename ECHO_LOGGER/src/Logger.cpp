#include "Logger/Logger.h"

namespace ECHO_LOGGER
{
    Logger &Logger::Get()
    {
        static Logger instance{};
        return instance;
    }

    void Logger::Init(bool console_log, bool retain_logs)
    {
        assert(!initialised && "Do not call initialise more than once");

        if (initialised)
        {
            std::cout << "Logger has already been initialised\n";
            return;
        }

        this->console_log = console_log;
        this->retain_logs = retain_logs;
        initialised = true;
    }

    Logger::LogTime::LogTime(const std::string &date) :
        day{date.substr(0, 3)}, day_number{date.substr(8, 2)},
        month{date.substr(4, 3)}, year{date.substr(20, 4)},
        time{date.substr(11, 8)}
    {
    }

    std::string Logger::DateTime()
    {
        auto time = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now());

        char buf[30];
        ctime_s(buf, sizeof(buf), &time);

        LogTime log_time{std::string{buf}};
        return std::format("{}-{}-{} {}", log_time.day_number, log_time.month,
            log_time.year, log_time.time);
    }
}
