#pragma once

#include "Logger.h"

#include <chrono>
#include <ctime>
#include <format>
#include <iostream>
#include <sstream>
#include <Windows.h>

constexpr WORD GREEN = 2;
constexpr WORD RED = 4;
constexpr WORD YELLOW = 6;
constexpr WORD WHITE = 7;

namespace ECHO_LOGGER
{
    template<typename ...Args>
    void Logger::Log(const std::string &message, Args && ...args)
    {
        assert(initialised && "The logger must be initialised before using");
        if (!initialised)
        {
            std::cout << "The logger must be initialised before using\n";
            return;
        }

        std::stringstream ss;
        ss << "ECHO [INFO]: " << DateTime() << " - " << std::vformat(message,
            std::make_format_args(std::forward<Args>(args)...)) << "\n";

        if (console_log)
        {
            HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(h_console, GREEN);
            std::cout << ss.str();
            SetConsoleTextAttribute(h_console, WHITE);
        }

        if (retain_logs)
        {
            log_entries.emplace_back(LogEntry::LogType::INFO, ss.str());
        }
    }

    template<typename ...Args>
    void Logger::Warn(const std::string &message, Args && ...args)
    {
        assert(initialised && "The logger must be initialised before using");
        if (!initialised)
        {
            std::cout << "The logger must be initialised before using\n";
            return;
        }

        std::stringstream ss;
        ss << "ECHO [WARN]: " << DateTime() << " - " << std::vformat(message,
            std::make_format_args(std::forward<Args>(args)...)) << "\n";

        if (console_log)
        {
            HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(h_console, YELLOW);
            std::cout << ss.str();
            SetConsoleTextAttribute(h_console, WHITE);
        }

        if (retain_logs)
        {
            log_entries.emplace_back(LogEntry::LogType::WARN, ss.str());
        }
    }

    template<typename ...Args>
    void Logger::Error(std::source_location location,
        const std::string &message, Args && ...args)
    {
        assert(initialised && "The logger must be initialised before using");
        if (!initialised)
        {
            std::cout << "The logger must be initialised before using\n";
            return;
        }

        std::stringstream ss;
        ss << "ECHO [ERROR]: " << DateTime() << " - " << std::vformat(message,
            std::make_format_args(std::forward<Args>(args)...)) <<
            "\nFUNC: " << location.function_name() <<
            "\nLINE: " << location.line() << "\n\n";

        if (console_log)
        {
            HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(h_console, RED);
            std::cout << ss.str();
            SetConsoleTextAttribute(h_console, WHITE);
        }

        if (retain_logs)
        {
            log_entries.emplace_back(LogEntry::LogType::ERR, ss.str());
        }
    }
}
