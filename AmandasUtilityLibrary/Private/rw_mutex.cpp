#include "stdafx.h"
#include "aul/rw_mutex.h"

namespace aul
{
    rw_mutex::rw_mutex() : _writelock(SRWLOCK_INIT)
    {
        _read_count.store(0);
        _is_writelocked.store(false);
    }

#if AUL_MSVC

    void rw_mutex::lock_read()
    {
        AcquireSRWLockExclusive(&_writelock);
        ++_read_count;
        ReleaseSRWLockExclusive(&_writelock);
    }

    void rw_mutex::lock_write()
    {
        AcquireSRWLockExclusive(&_writelock);
        _waitlock.wait([this]() { return _read_count > 0; });
        _is_writelocked.store(true);
    }

    void rw_mutex::unlock_read()
    {
        _waitlock.wake_all([this]() {
            --_read_count;
            return _read_count == 0;
        });
    }

    void rw_mutex::unlock_write()
    {
        _is_writelocked.store(false);
        ReleaseSRWLockExclusive(&_writelock);
    }

#elif AUL_STD

    void rw_mutex::lock_read()
    {
        std::lock_guard<std::mutex> lock(_writelock);
        ++_read_count;
    }

    void rw_mutex::lock_write()
    {
        _writelock.lock();
        _is_writelocked.store(true);
        _waitlock.wait([this]() {return _read_count > 0; });
    }

    void rw_mutex::unlock_read()
    {
        _waitlock.wake_all([this]() { return --_read_count == 0; });
    }

    void rw_mutex::unlock_write()
    {
        _is_writelocked.store(false);
        _writelock.unlock();
    }

#endif // AUL_MSVC
}
