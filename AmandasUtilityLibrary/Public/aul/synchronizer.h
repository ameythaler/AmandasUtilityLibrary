#pragma once

#if AUL_MSVC

#include "Windows.h"

#elif AUL_STD

#include <mutex>
#include <condition_variable>

#endif // AUL_MSVC

#include <functional>
#include <atomic>

namespace aul
{
    class synchronizer
    {
    public:
        synchronizer();

        void wait(std::function<bool(void)> wait_enter_condition);
        void wake_one(std::function<bool(void)> wake_condition);
        void wake_all(std::function<bool(void)> wake_condition);

        inline uint32 get_num_threads_waiting() const { return _num_waiting; };

    private:
#if AUL_MSVC

        SRWLOCK _waitlock;
        CONDITION_VARIABLE _waitvar;

#elif AUL_STD

        std::mutex _waitmutex;
        std::condition_variable _waitvar;

#endif // AUL_MSVC

        mutable std::atomic<uint32> _num_waiting;
    };
}