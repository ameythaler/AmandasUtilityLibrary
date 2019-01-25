#include "stdafx.h"
#include "aul/timer.h"

#if AUL_WIN
#include <windows.h>
#endif // AUL_WIN

namespace
{
#if AUL_WIN
    const aul::int64 _TO_MILLISECONDS_ = 10000;
    const aul::int64 _TO_MICROSECONDS_ = 10;

    void _raw_time_(aul::int64& raw_time)
    {
        LARGE_INTEGER cur_time;
        QueryPerformanceCounter(&cur_time);
        raw_time = cur_time.QuadPart;
    }
#elif AUL_POSIX
    typedef std::chrono::high_resolution_clock::time_point _time_point_;
    typedef std::chrono::high_resolution_clock _clock_;
#endif
}

namespace aul
{
    AUL_IMPLEMENT_SINGLETON(timer);

    timer::timer()
    {
#if AUL_WIN
        synchronize();
#elif // AUL_POSIX
        start_time = _clock_::now();
#endif // AUL_WIN
    }

    timer::~timer()
    {
        
    }

    void timer::synchronize()
    {
#if AUL_WIN
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        _frequency = frequency.QuadPart;
#endif // AUL_WIN
    }

    int64 timer::milliseconds() const
    {
#if AUL_WIN

        int64 raw_time;
        _raw_time_(raw_time);
        return raw_time / _TO_MILLISECONDS_;

#elif AUL_POSIX

        _time_point_ cur_time = _clock_::now();
        std::chrono::milliseconds dur = std::chrono::duration_cast<std::chrono::milliseconds>(cur_time - _start_time);
        return dur.count();

#endif
    }

    int64 timer::microseconds() const
    {
#if AUL_WIN

        int64 raw_time;
        _raw_time_(raw_time);
        return raw_time / _TO_MICROSECONDS_;

#elif AUL_POSIX

        _time_point_ cur_time = _clock_::now();
        std::chrono::microseconds dur = std::chrono::duration_cast<std::chrono::microseconds>(cur_time - _start_time);
        return dur.count();

#endif // AUL_WIN
    }
}