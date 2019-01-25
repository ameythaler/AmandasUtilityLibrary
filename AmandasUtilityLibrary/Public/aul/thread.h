#pragma once

#if AUL_MSVC

#include "Windows.h"

#elif AUL_STD

#include <thread>
#include <atomic>

#endif // AUL_MSVC

#include <functional>
#include "aul/timer.h"

namespace aul
{
    inline void yield_thread()
    {
#if AUL_MSVC
        SwitchToThread();
#elif AUL_STD
        std::this_thread::yield();
#endif // AUL_MSVC
    }

    inline void sleep_thread(uint32 milliseconds)
    {
#if AUL_MSVC
        Sleep(milliseconds);
#elif AUL_STD
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
#endif // AUL_MSVC
    }

#if AUL_MSVC
    void sleep_thread_until(uint64 milliseconds);
#elif AUL_STD
    void sleep_thread_until(uint64 milliseconds)
    {
        sleep_thread((uint32)(milliseconds - timer::instance().milliseconds()));
    }
#endif // AUL_MSVC

#define AUL_THREAD_START_FUNC(func_name) int32 func_name(void)

    typedef std::function<int32(void)> thread_start_func;

    class thread
    {
        AUL_DISABLECOPYING(thread);
    public:
        thread();
        thread(const thread_start_func& start_func);
        ~thread();
        bool is_exited() const;
        bool is_started() const;
        void start(const thread_start_func& start_func);
        void join();
        void detach();
        inline thread_id get_thread_id() const { return _thread_id; }
        static thread_id this_thread_id();

        bool operator==(const thread& rhs) const;
        bool operator!=(const thread& rhs) const;

    private:
#if AUL_MSVC
        HANDLE _thread_handle;
        mutable LONG _is_started;
#elif AUL_STD
        std::atomic<bool> _is_started;
        std::atomic<bool> _is_running;
        std::thread _thread;
        std::thread::native_handle_type _thread_handle
#endif // AUL_MSVC
        thread_start_func _start_func;
        thread_id _thread_id;

#if AUL_MSVC
        static DWORD WINAPI thread_start(LPVOID thread_params);
#elif AUL_STD
        static void thread_start(thread* thread_);
#endif // AUL_MSVC
    };
}