#pragma once

#if AUL_MSVC

#include "Windows.h"

#elif AUL_STD

#include <mutex>

#endif // AUL_MSVC

#include <atomic>
#include "aul/lockable.h"
#include "aul/synchronizer.h"

namespace aul
{
    class rw_recursive_mutex : public rw_lockable
    {
    public:
        rw_recursive_mutex();
#if AUL_MSVC
        ~rw_recursive_mutex() { DeleteCriticalSection(&_writelock); }
#endif // AUL_MSVC
        virtual void lock_read();
        virtual void lock_write();
        virtual void unlock_read();
        virtual void unlock_write();
        inline virtual uint32_t get_readlock_count() const { return _read_count; }
        inline virtual bool is_writelocked() const { return _writelock_count > 0; }

    private:
#if AUL_MSVC
        CRITICAL_SECTION _writelock;
#elif AUL_STD
        std::recursive_mutex _writelock;
#endif // AUL_MSVC
        synchronizer _waitlock;
        std::atomic<uint32_t> _read_count;
        std::atomic<uint32_t> _writelock_count;
    };
}

#include "aul/scope_lock.h"