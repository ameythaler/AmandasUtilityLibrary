#include "stdafx.h"
#include "aul/thread.h"

namespace aul
{
#if AUL_MSVC

    void sleep_thread_until(uint64 milliseconds)
    {
        uint64 cur_time = aul::timer::instance().milliseconds();
        if (milliseconds > cur_time)
        {
            sleep_thread((uint32)(milliseconds - cur_time));
        }
    }

    DWORD WINAPI thread::thread_start(LPVOID thread_params)
    {
        thread* thread_ = (thread*)thread_params;
        thread_start_func start_func = thread_->_start_func;
        InterlockedExchange(&thread_->_is_started, 1);
        return start_func();
    }

    thread::thread()
        : _thread_handle(nullptr)
        , _thread_id(0)
        , _start_func(nullptr)
        , _is_started(0)
    {
    }

    thread::thread(const thread_start_func& start_func)
    {
        start(start_func);
    }

    thread::~thread()
    {
        if (_thread_handle && _start_func) // _start_func will be null if thread was created via this_thread()
        {
            join();
        }
    }

    bool thread::is_exited() const
    {
        return WaitForSingleObject(_thread_handle, 0) == WAIT_OBJECT_0;
    }

    bool thread::is_started() const
    {
        return InterlockedCompareExchange(&_is_started, 0, 0) == 1;
    }

    void thread::start(const thread_start_func& start_func)
    {
        _start_func = start_func;
        InterlockedExchange(&_is_started, 0);
        _thread_handle = CreateThread(NULL, 0, &thread_start, (LPVOID)this, 0, (LPDWORD)&_thread_id);
        while (InterlockedCompareExchange(&_is_started, 1, 1) != 1)
        {
        }
    }

    void thread::join()
    {
        if (_thread_handle)
        {
            WaitForSingleObject(_thread_handle, INFINITE);
        }
        _thread_handle = nullptr;
    }

    void thread::detach()
    {
        if (_thread_handle)
        {
            CloseHandle(_thread_handle);
        }
        _thread_handle = nullptr;
    }

    thread_id thread::this_thread_id()
    {
        return GetCurrentThreadId();
    }

#elif AUL_STD

    void thread::thread_start(thread* thread_)
    {
        thread_start_func start_func = thread_->_start_func;
        thread_->_is_running.store(true);
        thread_->_is_started.store(true);
        start_func();
    }

    thread::thread()
        : _is_started(false)
        , _thread_handle(nullptr)
    {

    }

    thread::thread(const thread_start_func& start_func)
    {
        start(start_func);
    }

    thread::~thread()
    {
        if (_thread.joinable())
        {
            _thread.join();
        }
    }

    bool thread::is_exited() const
    {
        return !_is_running;
    }

    bool thread::is_started() const
    {
        return _is_started;
    }

    void thread::start(const thread_start_func& start_func)
    {
        _is_started.store(false);
        _thread = std::thread(thread_start, this);
        _thread_handle = _thread.native_handle();
        _thread_id = _thread.get_id();
        while (!_is_started)
        {
        }
    }

    void thread::join()
    {
        _thread.join();
    }

    void thread::detach()
    {
        _thread.detach();
    }

    thread_id thread::this_thread_id()
    {
        return std::this_thread::get_id();
    }

#endif // AUL_MSVC

    bool thread::operator==(const thread& rhs) const
    {
        return _thread_id == rhs._thread_id;
    }

    bool thread::operator!=(const thread& rhs) const
    {
        return _thread_id != rhs._thread_id;
    }
}