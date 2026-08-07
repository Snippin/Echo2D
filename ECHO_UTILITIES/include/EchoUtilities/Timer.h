#pragma once

#include <sol/forward.hpp>

#include <chrono>

namespace ECHO_UTIL
{
    class Timer
    {
    public:
        Timer() = default;
        ~Timer() = default;

        static void CreateLuaBind(sol::state &lua);

        void Start();
        void Stop();
        void Pause();
        void Resume();
        void Restart();

        int64_t ElapsedMS() const;
        int64_t ElapsedSec() const;

        bool IsRunning() const;
        bool IsPaused() const;

    private:
        std::chrono::time_point<std::chrono::steady_clock> start_point;
        std::chrono::time_point<std::chrono::steady_clock> paused_point;

        bool is_running{};
        bool is_paused{};
    };
}
