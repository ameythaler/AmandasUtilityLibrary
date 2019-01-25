#pragma once

#include "aul/platform.h"
#include "aul/macros.h"
#include <inttypes.h>
#include <wchar.h>
#include <sstream>
#include <vector>

namespace aul
{
    typedef std::basic_string<char> mb_string;
    typedef std::basic_string<wchar_t> wide_string;
    typedef std::basic_string<char16_t> wide_string16;
    typedef std::basic_stringstream<char> mb_stringstream;
    typedef std::basic_stringstream<wchar_t> wide_stringstream;
    typedef std::basic_ostream<char> mb_ostream;
    typedef std::basic_ostream<wchar_t> wide_ostream;
    typedef std::basic_stringbuf<char> mb_stringbuf;
    typedef std::basic_stringbuf<wchar_t> wide_stringbuf;

#if AUL_UNICODE
    typedef wide_string aul_string;
    typedef wide_stringstream aul_stringstream;
    typedef wchar_t aul_char;
    typedef wide_ostream aul_ostream;
    typedef wide_stringbuf aul_stringbuf;
#define AUL_TEXT(x) AUL_CAT(L, x)
#else
    typedef mb_string aul_string;
    typedef mb_stringstream aul_stringstream;
    typedef char aul_char;
    typedef mb_ostream aul_ostream;
    typedef mb_stringbuf aul_stringbuf;
#define AUL_TEXT(x) x
#endif // AUL_UNICODE

    typedef uint8_t byte;
    typedef uint16_t word;
    typedef uint32_t dword;
    typedef uint64_t qword;

    typedef uint8_t uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;

    typedef int8_t int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef int64_t int64;

    typedef uint32 thread_id;

    // #TODO Delete this if unnecessary
    struct local_char_ptr
    {
        char* _ptr;

        local_char_ptr() : _ptr(nullptr) { }
        local_char_ptr(unsigned int size) { _ptr = new char[size]; }
        local_char_ptr(const aul_string& str);
        ~local_char_ptr() { delete[] _ptr; }
        operator char*() { return _ptr; }
        char** operator&() { return &_ptr; }
        char operator*() { return *_ptr; }
        char operator[](unsigned int index) { return _ptr[index]; }
        operator aul_string() const;
    };
}