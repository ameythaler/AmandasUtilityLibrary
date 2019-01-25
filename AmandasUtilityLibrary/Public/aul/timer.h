#pragma once

#include "aul/types.h"
#include "aul/macros.h"
#if AUL_POSIX
#include <chrono>
#endif // AUL_POSIX

namespace aul
{
    class timer
    {
        AUL_DECLARE_SINGLETON(timer);
    public:
        void synchronize();
        int64 milliseconds() const;
        int64 microseconds() const;

    private:
#if AUL_WIN

        int64 _frequency;

#elif AUL_POSIX

        std::chrono::high_resolution_clock::time_point _start_time;

#endif // AUL_WIN
    };
}