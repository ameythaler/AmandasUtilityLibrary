#include "stdafx.h"
#include "aul/rw_recursive_mutex.h"

namespace aul
{
    rw_recursive_mutex::rw_recursive_mutex()
    {
#if AUL_MSVC
        InitializeCriticalSectionAndSpinCount(&_writelock, 100);
#endif // AUL_MSVC
        _read_count.store(0);
        _writelock_count.store(0);
    }

    void rw_recursive_mutex::lock_read()
    {
#if AUL_MSVC
        EnterCriticalSection(&_writelock);
#elif AUL_STD
        std::lock_guard<std::recursive_mutex> lock(_writelock);
#endif // AUL_MSVC
        ++_read_count;
#if AUL_MSVC
        LeaveCriticalSection(&_writelock);
#endif // AUL_MSVC
    }

    void rw_recursive_mutex::lock_write()
    {
#if AUL_MSVC
        EnterCriticalSection(&_writelock);
#elif AUL_STD
        _writelock.lock();
#endif // AUL_MSVC
        ++_writelock_count;
        _waitlock.wait([this]() { return _read_count > 0; });
    }

    void rw_recursive_mutex::unlock_read()
    {
        _waitlock.wake_all([this]() { return --_read_count == 0; });
    }

    void rw_recursive_mutex::unlock_write()
    {
        --_writelock_count;
#if AUL_MSVC
        LeaveCriticalSection(&_writelock);
#elif AUL_STD
        _writelock.unlock();
#endif // AUL_MSVC
    }
}