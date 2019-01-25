#pragma once

namespace aul
{
    wide_string16 to_wide_string16_from_wide_string(const wide_string& str);
    wide_string from_wide_string16_to_wide_string(const wide_string16& str);
    wide_string16 to_wide_string16_from_mb_string(const mb_string& str);
    mb_string from_wide_string16_To_mb_string(const wide_string16& str);

    inline wide_string16 to_wide_string16(const aul_string& str)
    {
#if AUL_UNICODE
        return to_wide_string16_from_wide_string(str);
#else // AUL_UNICODE
        return to_wide_string16_from_mb_string(str);
#endif // AUL_UNICODE
    }
    inline aul_string from_wide_string16(const wide_string16& str)
    {
#if AUL_UNICODE
        return from_wide_string16_to_wide_string(str);
#else // AUL_UNICDOE
        return from_wide_string16_To_mb_string(str);
#endif // AUL_UNICDOE
    }

    void wide_string_to_mb_char_array(char** mb_char_array, const wide_string& str);
    void wide_string_to_mb_string(mb_string& out, const wide_string& str);
    void mb_char_array_to_wide_string(wide_string& str, const char* mb_char_array);
    void mb_string_to_wide_string(wide_string& out, const mb_string& str);

#if AUL_UNICODE
    inline void aul_string_to_mb_char_array(char** mb_char_array, const aul_string& str)
    {
        wide_string_to_mb_char_array(mb_char_array, str);
    }

    inline void mb_char_array_to_aul_string(aul_string& str, const char* mb_char_array)
    {
        mb_char_array_to_wide_string(str, mb_char_array);
    }

    inline aul_string wide_string_to_aul_string(const wide_string& str)
    {
        return str;
    }

    inline wide_string aul_string_to_wide_string(const aul_string& str)
    {
        return str;
    }

    inline aul_string mb_string_to_aul_string(const mb_string& str)
    {
        aul_string out;
        mb_string_to_wide_string(out, str);
        return out;
    }

    inline mb_string aul_string_to_mb_string(const aul_string& str)
    {
        mb_string out;
        wide_string_to_mb_string(out, str);
        return out;
    }
#else // AUL_UNICODE
    inline void aul_string_to_mb_char_array(char** mb_char_array, const aul_string& str)
    {
        *mb_char_array = str.c_str();
    }

    inline void mb_char_array_to_aul_string(aul_string& str, const char* mb_char_array)
    {
        str = mb_char_array;
    }

    inline aul_string wide_string_to_aul_string(const wide_string& str)
    {
        aul_string out;
        wide_string_to_mb_string(out, str);
        return out;
    }

    inline wide_string aul_string_to_wide_string(const aul_string& str)
    {
        mb_string out;
        mb_string_to_wide_string(out, str);
        return out;
    }

    inline aul_string mb_string_to_aul_string(const mb_string& str)
    {
        return str;
    }

    inline mb_string aul_string_to_mb_string(const aul_string& str)
    {
        return str;
    }
#endif // AUL_UNICODE

    template<typename T_CHAR_TYPE>
    bool char_in_char_array(T_CHAR_TYPE c, const T_CHAR_TYPE* s, size_t len)
    {
        size_t cur = 0;
        while (cur < len)
        {
            if (s[cur] == c)
            {
                return true;
            }
            ++cur;
        }
        return false;
    }

    std::vector<mb_string> split_mb_string(const mb_string& str, char delim, bool keep_delim);
    std::vector<wide_string> split_wide_string(const wide_string& str, wchar_t delim, bool keep_delim);
    inline std::vector<aul_string> split_aul_string(const aul_string& str, aul_char delim, bool keep_delim)
    {
#if AUL_UNICODE
        return split_wide_string(str, delim, keep_delim);
#else // AUL_UNICODE
        return split_mb_string(str, delim, keep_delim);
#endif // AUL_UNICODE
    }

    std::vector<mb_string> split_mb_string(const mb_string& str, const char* delims, bool keep_delims);
    std::vector<wide_string> split_wide_string(const wide_string& str, const wchar_t* delims, bool keep_delims);
    inline std::vector<aul_string> split_aul_string(const aul_string& str, const aul_char* delims, bool keep_delims)
    {
#if AUL_UNICODE
        return split_wide_string(str, delims, keep_delims);
#else // AUL_UNICODE
        return split_mb_string(str, delims, keep_delims);
#endif // AUL_UNICODE
    }

    void mb_string_to_upper(mb_string& str);
    void wide_string_to_upper(wide_string& str);
    inline void aul_string_to_upper(aul_string& str)
    {
#if AUL_UNICODE
        wide_string_to_upper(str);
#else // AUL_UNICODE
        mb_string_to_upper(str);
#endif // AUL_UNICODE
    }

    mb_string mb_string_as_upper(const mb_string& str);
    wide_string wide_string_as_upper(const wide_string& str);
    inline aul_string ault_string_as_upper(const aul_string& str)
    {
#if AUL_UNICODE
        return wide_string_as_upper(str);
#else // AUL_UNICODE
        return mb_string_as_upper(str);
#endif // AUL_UNICODE
    }

    void mb_string_to_lower(mb_string& str);
    void wide_string_to_lower(wide_string& str);
    inline void aul_string_to_lower(aul_string& str)
    {
#if AUL_UNICODE
        wide_string_to_lower(str);
#else // AUL_UNICODE
        mb_string_to_lower(str);
#endif // AUL_UNICODE
    }

    mb_string mb_string_as_lower(const mb_string& str);
    wide_string wide_string_as_lower(const wide_string& str);
    inline aul_string ault_string_as_lower(const aul_string& str)
    {
#if AUL_UNICODE
        return wide_string_as_lower(str);
#else // AUL_UNICODE
        return mb_string_as_lower(str);
#endif // AUL_UNICODE
    }
}