#pragma once

#include "aul/platform.h"
#include "aul/types.h"
#include "aul/macros.h"

namespace aul
{
    typedef size_t memstack_mark;

    class memstack
    {
    public:
        memstack(size_t stack_size = 2000u);
        ~memstack();
        AUL_DISABLECOPYING(memstack);

        inline memstack_mark mark() { return _top; }
        template<typename T> T* push(size_t count = 1);
        inline void pop(memstack_mark mark) { _top = mark; }

        inline size_t get_stack_size() const { return _stack_size; }

    private:
        byte* _stack;
        size_t _top;
        size_t _stack_size;
    };
}

#include "aul/impl/memstack.hpp"