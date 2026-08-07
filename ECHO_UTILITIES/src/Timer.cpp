#include "EchoUtilities/Timer.h"

#include <sol/sol.hpp>

namespace ECHO_UTIL
{
    void Timer::CreateLuaBind(sol::state &lua)
    {
        lua.new_usertype<Timer>(
            "Timer",
            sol::call_constructor,
            sol::factories([]() { return Timer{}; }),
            "Start", &Timer::Start,
            "Stop", &Timer::Stop,
            "Pause", &Timer::Pause,
            "Resume", &Timer::Resume,
            "Restart", &Timer::Restart,
            "IsPaused", &Timer::IsPaused,
            "IsRunning", &Timer::IsRunning,
            "ElapsedMS", &Timer::ElapsedMS,
            "ElapsedSec", &Timer::ElapsedSec
        );
    }

    void Timer::Start()
    {
        if (!is_running)
        {
            is_running = true;
            is_paused = false;
            start_point = std::chrono::steady_clock::now();
        }
    }

    void Timer::Stop()
    {
        if (is_running)
        {
            is_running = false;
        }
    }

    void Timer::Pause()
    {
        if (is_running && !is_paused)
        {
            is_paused = true;
            paused_point = std::chrono::steady_clock::now();
        }
    }

    void Timer::Resume()
    {
        if (is_running && is_paused)
        {
            is_paused = false;
            start_point += std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - paused_point);
        }
    }

    void Timer::Restart()
    {
        if (is_running)
        {
            Stop();
        }

        Start();
    }

    int64_t Timer::ElapsedMS() const
    {
        if (is_running)
        {
            if (is_paused)
            {
                return std::chrono::duration_cast<std::chrono::milliseconds>(
                    paused_point - start_point).count();
            }
            else
            {
                return std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - start_point).count();
            }
        }

        return 0;
    }

    int64_t Timer::ElapsedSec() const
    {
        return ElapsedMS() / 1000;
    }

    bool Timer::IsRunning() const
    {
        return is_running;
    }

    bool Timer::IsPaused() const
    {
        return is_paused;
    }
}
