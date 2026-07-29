#pragma once

#include "Logger.h"

#include <fmt/format.h>

#include <iostream>
#include <sstream>

namespace ECHO_LOGGER
{
    template<typename ...Args>
    void Logger::Log(std::string_view message, Args && ...args)
    {
        assert(initialised && "The logger must be initialised before using");
        if (!initialised)
        {
            std::cout << "The logger must be initialised before using\n";
            return;
        }

        std::stringstream ss;
        ss << "ECHO [INFO]: " << DateTime() << " - " << fmt::vformat(message,
            fmt::make_format_args(args...)) << "\n";

        if (console_log)
        {
            ConsoleLog(LogEntry::LogType::INFO, ss.str());
        }

        if (retain_logs)
        {
            log_entries.emplace_back(LogEntry::LogType::INFO, ss.str());
        }
    }

    template<typename ...Args>
    void Logger::Warn(std::string_view message, Args && ...args)
    {
        assert(initialised && "The logger must be initialised before using");
        if (!initialised)
        {
            std::cout << "The logger must be initialised before using\n";
            return;
        }

        std::stringstream ss;
        ss << "ECHO [WARN]: " << DateTime() << " - " << fmt::vformat(message,
            fmt::make_format_args(args...)) << "\n";

        if (console_log)
        {
            ConsoleLog(LogEntry::LogType::WARN, ss.str());
        }

        if (retain_logs)
        {
            log_entries.emplace_back(LogEntry::LogType::WARN, ss.str());
        }

    }

    template<typename ...Args>
    void Logger::Error(std::source_location location, std::string_view message, 
        Args && ...args)
    {
        assert(initialised && "The logger must be initialised before using");
        if (!initialised)
        {
            std::cout << "The logger must be initialised before using\n";
            return;
        }

        std::stringstream ss;
        ss << "ECHO [ERROR]: " << DateTime() << " - " << fmt::vformat(message,
            fmt::make_format_args(args...)) <<
            "\nFUNC: " << location.function_name() <<
            "\nLINE: " << location.line() << "\n\n";

        if (console_log)
        {
            ConsoleLog(LogEntry::LogType::ERR, ss.str());
        }

        if (retain_logs)
        {
            log_entries.emplace_back(LogEntry::LogType::ERR, ss.str());
        }
    }
}
