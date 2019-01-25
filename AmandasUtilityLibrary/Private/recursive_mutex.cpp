#include "stdafx.h"
#include "aul/recursive_mutex.h"
#include "aul/thread.h"

namespace aul
{
    recursive_mutex::recursive_mutex()
    {
#if AUL_MSVC
        InitializeCriticalSectionAndSpinCount(&_lock, 100);
#endif // AUL_MSVC
        _lock_count.store(0);
        _locked_thread.store(0);
    }

    void recursive_mutex::lock()
    {
#if AUL_MSVC
        EnterCriticalSection(&_lock);
#elif AUL_STD
        _lock.lock();
#endif // AUL_MSVC
        ++_lock_count;
        _locked_thread = thread::this_thread_id();
    }

    void recursive_mutex::unlock()
    {
#if AUL_MSVC
        LeaveCriticalSection(&_lock);
#elif AUL_STD
        _lock.unlock();
#endif // AUL_MSVC
        --_lock_count;
        if (_lock_count == 0) _locked_thread.store(0);
    }

    bool recursive_mutex::is_same_thread() const
    {
        return thread::this_thread_id() == _locked_thread;
    }
}