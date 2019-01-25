#pragma once

#if AUL_MSVC

#include "Windows.h"

#elif AUL_STD

#include <recursive_mutex.h>

#endif // AUL_MSVC

#include <atomic>
#include "aul/lockable.h"

namespace aul
{
    class recursive_mutex : public lockable
    {
    public:
        recursive_mutex();
#if AUL_MSVC
        ~recursive_mutex() { DeleteCriticalSection(&_lock); }
#endif // AUL_MSVC
        void lock();
        void unlock();
        bool is_locked() const { return _lock_count > 0 && !is_same_thread(); }
        uint32_t lock_count() const { return _lock_count; }

    private:
#if AUL_MSVC
        CRITICAL_SECTION _lock;
#elif AUL_STD
        std::recursive_mutex _lock;
#endif // AUL_MSVC
        std::atomic<thread_id> _locked_thread;
        std::atomic<uint32> _lock_count;

        bool is_same_thread() const;
    };
}

#include "aul/scope_lock.h"