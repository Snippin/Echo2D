#include "Logger/Logger.h"

#include <chrono>
#include <format>
#include <iostream>
#include <Windows.h>

namespace ECHO_LOGGER
{
    static constexpr WORD GREEN = 2;
    static constexpr WORD RED = 4;
    static constexpr WORD YELLOW = 6;
    static constexpr WORD WHITE = 7;

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

    void Logger::ConsoleLog(LogEntry::LogType type, std::string_view message)
    {
        HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);

        switch (type)
        {
            using enum ECHO_LOGGER::LogEntry::LogType;
        case INFO:
            SetConsoleTextAttribute(h_console, GREEN);
            break;
        case WARN:
            SetConsoleTextAttribute(h_console, YELLOW);
            break;
        case ERR:
            SetConsoleTextAttribute(h_console, RED);
            break;
        case NONE:
            break;
        default:
            ECHO_ERROR("No `LogEntry::LogType` is invalid");
            return;
        }

        std::cout << message;
        SetConsoleTextAttribute(h_console, WHITE);
    }

    std::string Logger::DateTime() const
    {
        auto now = std::chrono::system_clock::now();
        return std::format("{:%H:%M:%S}", now);
    }
}
