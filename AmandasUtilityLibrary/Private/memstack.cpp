#include "stdafx.h"
#include "aul/memstack.h"

namespace aul
{
    memstack::memstack(size_t stack_size)
    {
        _stack = (byte*)malloc(stack_size);
        _top = 0;
        _stack_size = stack_size;
    }

    memstack::~memstack()
    {
        free(_stack);
        _stack = nullptr;
        _top = 0;
        _stack_size = 0;
    }
}