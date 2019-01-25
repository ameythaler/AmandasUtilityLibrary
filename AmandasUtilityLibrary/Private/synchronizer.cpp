#include "stdafx.h"
#include "aul/synchronizer.h"

namespace aul
{

#if AUL_MSVC

    synchronizer::synchronizer()
    {
        InitializeSRWLock(&_waitlock);
        InitializeConditionVariable(&_waitvar);
        _num_waiting.store(0);
    }


    void synchronizer::wait(std::function<bool(void)> wait_enter_condition)
    {
        AcquireSRWLockExclusive(&_waitlock);
        ++_num_waiting;
        if (wait_enter_condition())
            SleepConditionVariableSRW(&_waitvar, &_waitlock, INFINITE, 0);
        --_num_waiting;
        ReleaseSRWLockExclusive(&_waitlock);
    }

    void synchronizer::wake_all(std::function<bool(void)> wake_condition)
    {
        AcquireSRWLockExclusive(&_waitlock);
        if (wake_condition())
            WakeAllConditionVariable(&_waitvar);
        ReleaseSRWLockExclusive(&_waitlock);
    }

    void synchronizer::wake_one(std::function<bool(void)> wake_condition)
    {
        AcquireSRWLockExclusive(&_waitlock);
        if (wake_condition())
            WakeConditionVariable(&_waitvar);
        ReleaseSRWLockExclusive(&_waitlock);
    }    

#elif AUL_STD

    synchronizer::synchronizer()
    {
        _num_waiting.store(0);
    }

    void synchronizer::wait(std::function<bool(void)> wait_enter_condition)
    {
        std::unique_lock<std::mutex> wait_lock(_waitmutex);
        ++_num_waiting;
        if (wait_enter_condition())
            _waitvar.wait(wait_lock);
        --_num_waiting;
    }

    void synchronizer::wake_all(std::function<bool(void)> wake_condition)
    {
        std::unique_lock<std::mutex> wait_lock(_waitmutex);
        if (wake_condition())
            _waitvar.notify_all();
    }

    void synchronizer::wake_one(std::function<bool(void)> wake_condition)
    {
        std::unique_lock<std::mutex> wait_lock(_waitmutex);
        if (wake_condition())
            _waitvar.notify_one();
    }

#endif // ETL_MSVC
}
