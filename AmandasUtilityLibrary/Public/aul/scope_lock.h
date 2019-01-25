#pragma once

#include "aul/lockable.h"

namespace aul
{
    struct scope_lock
    {
        scope_lock(lockable& lockable_) : _lockable(lockable_)
        {
            _lockable.lock();
        }
        ~scope_lock()
        {
            _lockable.unlock();
        }
        
    private:
        lockable& _lockable;
    };

    struct rw_scope_readlock
    {
        rw_scope_readlock(rw_lockable& lockable_) : _lockable(lockable_)
        {
            _lockable.lock_read();
        }
        ~rw_scope_readlock()
        {
            _lockable.unlock_read();
        }

    private:
        rw_lockable& _lockable;
    };

    struct rw_scope_writelock
    {
        rw_scope_writelock(rw_lockable& lockable_) : _lockable(lockable_)
        {
            _lockable.lock_write();
        }
        ~rw_scope_writelock()
        {
            _lockable.unlock_write();
        }

    private:
        rw_lockable& _lockable;
    };
}