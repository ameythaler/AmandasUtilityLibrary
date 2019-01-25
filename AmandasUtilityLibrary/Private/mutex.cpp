#include "stdafx.h"
#include "aul/mutex.h"

namespace aul
{
    void mutex::lock()
    {
#if AUL_MSVC
        AcquireSRWLockExclusive(&_lock);
#elif AUL_STD
        _lock.lock();
#endif // AUL_MSVC
        _is_locked.store(true);
    }

    void mutex::unlock()
    {
#if AUL_MSVC
        ReleaseSRWLockExclusive(&_lock);
#elif AUL_STD
        _lock.unlock();
#endif // AUL_MSVC
        _is_locked.store(false);
    }
}