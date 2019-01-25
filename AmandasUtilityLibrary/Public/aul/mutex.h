#pragma once

#if AUL_MSVC

#include "Windows.h"

#elif // AUL_STD

#include <mutex.h>

#endif // AUL_MSVC

#include <atomic>
#include "aul/lockable.h"

namespace aul
{
    class mutex : public lockable
    {
    public:
#if AUL_MSVC
        mutex() : _lock(SRWLOCK_INIT) { _is_locked.store(false); }
#endif // AUL_MSVC
        void lock();
        void unlock();
        bool is_locked() const { return _is_locked.load(); }

    private:
#if AUL_MSVC
        SRWLOCK _lock;
#elif AUL_STD
        std::mutex _lock;
#endif // AUL_MSVC
        std::atomic<bool> _is_locked;
    };
}

#include "aul/scope_lock.h"