#pragma once

#if AUL_MSVC

#include "Windows.h"

#elif AUL_STD

#include <mutex>
#include <thread>

#endif // AUL_MSVC

#include "aul/lockable.h"
#include "aul/synchronizer.h"
#include <atomic>

namespace aul
{
    class rw_mutex : public rw_lockable
    {
    public:
        rw_mutex();
        virtual void lock_read();
        virtual void lock_write();
        virtual void unlock_read();
        virtual void unlock_write();
        inline virtual uint32 get_readlock_count() const { return _read_count; }
        inline virtual bool is_writelocked() const { return _is_writelocked; }

    private:
#if AUL_MSVC

        SRWLOCK _writelock;

#elif AUL_STD

        std::mutex _writelock;

#endif // AUL_MSVC
        synchronizer _waitlock;
        std::atomic<uint32> _read_count;
        std::atomic<bool> _is_writelocked;
    };
}

#include "aul/scope_lock.h"