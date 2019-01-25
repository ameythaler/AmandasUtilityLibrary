#include "stdafx.h"
#include "aul/string.h"

namespace aul
{
    local_char_ptr::local_char_ptr(const aul_string& str)
    {
        aul_string_to_mb_char_array(&_ptr, str);
    }

    local_char_ptr::operator aul_string() const
    {
        aul_string str;
        mb_char_array_to_aul_string(str, _ptr);
        return str;
    }
}