namespace aul
{
    template<typename T>
    T* memstack::push(size_t count)
    {
        size_t size = count * sizeof(T);
        size_t top = _top;
        if (top + size > _stack_size)
            return nullptr;
        _top += size;
        return (T*)&_stack[top];
    }
}