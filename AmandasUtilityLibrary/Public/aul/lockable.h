#pragma once

namespace aul
{
    class lockable
    {
    public:
        virtual void lock() = 0;
        virtual void unlock() = 0;
        virtual bool is_locked() = 0;
    };

    class rw_lockable
    {
    public:
        virtual void lock_read() = 0;
        virtual void lock_write() = 0;
        virtual void unlock_read() = 0;
        virtual void unlock_write() = 0;
        virtual uint32 get_readlock_count() const = 0;
        virtual bool is_writelocked() const = 0;
    };
}