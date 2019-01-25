#include "stdafx.h"
#include "aul/string.h"

#if AUL_MSVC
#pragma warning(push)
#pragma warning(disable: 4996) // disable unbounded string deprecation warnings
#endif // AUL_MSVC

namespace
{
    template<typename T_SIZE_TYPE>
    T_SIZE_TYPE _string_length_(const char* s)
    {
        return (T_SIZE_TYPE)strlen(s);
    }

    template<typename T_SIZE_TYPE>
    T_SIZE_TYPE _string_length_(const wchar_t* s)
    {
        return (T_SIZE_TYPE)wcslen(s);
    }

    template<typename T_CHAR_TYPE, typename T_STRING_TYPE, typename T_SIZE_TYPE>
    std::vector<T_STRING_TYPE> _split_string_(const T_STRING_TYPE& str, T_CHAR_TYPE delim, bool keep_delim)
    {
        std::vector<T_STRING_TYPE> ret_val;
        T_SIZE_TYPE start = 0;
        T_SIZE_TYPE str_len = str.length();
        T_SIZE_TYPE cur;
        for (cur = 0; cur < str_len; ++cur)
        {
            if (str[cur] == delim) // We've reached a delimiter
            {
                if (cur - start > 0) // There's actually string content here
                {
                    if (keep_delim)
                    {
                        // We need to find the next non-delim char
                        while (cur < str_len && str[cur] == delim)
                        {
                            ++cur;
                        }
                        ret_val.push_back(str.substr(start, cur - start));
                        start = cur;
                    }
                    else
                    {
                        ret_val.push_back(str.substr(start, cur - start));
                        start = cur + 1;
                    }
                }
                else if (!keep_delim)
                {
                    ++start; // Discard all delimiter characters
                }
            }
        }
        if (cur - start > 0 && start < str_len) // Handle left over characters
        {
            ret_val.push_back(str.substr(start, cur - start));
        }
        return ret_val;
    }

    template<typename T_CHAR_TYPE, typename T_STRING_TYPE, typename T_SIZE_TYPE>
    std::vector<T_STRING_TYPE> _split_string_(const T_STRING_TYPE& str, const T_CHAR_TYPE* delims, bool keep_delims)
    {
        std::vector<T_STRING_TYPE> ret_val;
        T_SIZE_TYPE start = 0;
        T_SIZE_TYPE str_len = str.length();
        T_SIZE_TYPE delim_len = _string_length_<T_SIZE_TYPE>(delims);
        T_SIZE_TYPE cur;
        for (cur = 0; cur < str_len; ++cur)
        {
            if (aul::char_in_char_array(str[cur], delims, delim_len)) // We've reached a delimiter
            {
                if (cur - start > 0) // There's actually string content here
                {
                    if (keep_delims)
                    {
                        // We need to find the next non-delim char
                        while (cur < str_len && aul::char_in_char_array(str[cur], delims, delim_len))
                        {
                            ++cur;
                        }
                        ret_val.push_back(str.substr(start, cur - start));
                        start = cur;
                    }
                    else
                    {
                        ret_val.push_back(str.substr(start, cur - start));
                        start = cur + 1;
                    }
                }
                else if (!keep_delims)
                {
                    ++start; // Discard all delimiter characters
                }
            }
        }
        if (cur - start > 0 && start < str_len) // Handle left over characters
        {
            ret_val.push_back(str.substr(start, cur - start));
        }
        return ret_val;
    }

    template<typename T_STRING_TYPE, typename T_CHAR_TYPE>
    void _string_to_upper_(T_STRING_TYPE& str)
    {
        const std::locale& loc = std::locale();
        for (T_STRING_TYPE::iterator char_iter = str.begin(); char_iter != str.end(); char_iter++)
        {
            *char_iter = std::use_facet<std::ctype<T_CHAR_TYPE>>(loc).toupper(*char_iter);
        }
    }

    template<typename T_STRING_TYPE, typename T_CHAR_TYPE>
    T_STRING_TYPE _string_as_upper_(const T_STRING_TYPE& str)
    {
        T_STRING_TYPE out_string = str;
        const std::locale& loc = std::locale();
        for (T_STRING_TYPE::iterator char_iter = out_string.begin(); char_iter != out_string.end(); char_iter++)
        {
            *char_iter = std::use_facet<std::ctype<T_CHAR_TYPE>>(loc).toupper(*char_iter);
        }
        return out_string;
    }

    template<typename T_STRING_TYPE, typename T_CHAR_TYPE>
    void _string_to_lower_(T_STRING_TYPE& str)
    {
        const std::locale& loc = std::locale();
        for (T_STRING_TYPE::iterator char_iter = str.begin(); char_iter != str.end(); char_iter++)
        {
            *char_iter = std::use_facet<std::ctype<T_CHAR_TYPE>>(loc).tolower(*char_iter);
        }
    }

    template<typename T_STRING_TYPE, typename T_CHAR_TYPE>
    T_STRING_TYPE _string_as_lower_(const T_STRING_TYPE& str)
    {
        T_STRING_TYPE out_string = str;
        const std::locale& loc = std::locale();
        for (T_STRING_TYPE::iterator char_iter = out_string.begin(); char_iter != out_string.end(); char_iter++)
        {
            *char_iter = std::use_facet<std::ctype<T_CHAR_TYPE>>(loc).tolower(*char_iter);
        }
        return out_string;
    }
}

namespace aul
{
    wide_string16 to_wide_string16_from_wide_string(const wide_string& str)
    {
        wide_string16 ret_val;
        for (wchar_t ch : str)
        {
            ret_val.push_back((char16_t)ch);
        }
        return ret_val;
    }

    wide_string from_wide_string16_to_wide_string(const wide_string16& str)
    {
        wide_string ret_val;
        for (char16_t ch : str)
            ret_val.push_back((wchar_t)ch);
        return ret_val;
    }

    wide_string16 to_wide_string16_from_mb_string(const mb_string& str)
    {
        wide_string wide_str;
        mb_string_to_wide_string(wide_str, str);
        return to_wide_string16_from_wide_string(wide_str);
    }

    mb_string from_wide_string16_To_mb_string(const wide_string16& str)
    {
        mb_string ret_val;
        wide_string wide_str = from_wide_string16_to_wide_string(str);
        wide_string_to_mb_string(ret_val, wide_str);
        return ret_val;
    }

    void wide_string_to_mb_char_array(char** mb_char_array, const wide_string& str)
    {
        *mb_char_array = new char[str.length() + 1];
        wcstombs(*mb_char_array, &str[0], str.length() + 1);
    }

    void wide_string_to_mb_string(mb_string& out, const wide_string& str)
    {
        out.resize(str.length());
        wcstombs(&out[0], &str[0], str.length());
    }

    void mb_char_array_to_wide_string(wide_string& str, const char* mb_char_array)
    {
        size_t str_len = strlen(mb_char_array);
        str.resize(str_len);
        mbstowcs(&str[0], mb_char_array, str_len);
    }

    void mb_string_to_wide_string(wide_string& out, const mb_string& str)
    {
        out.resize(str.length());
        mbstowcs(&out[0], &str[0], str.length());
    }

    std::vector<mb_string> split_mb_string(const mb_string& str, char delim, bool keep_delim)
    {
        return _split_string_<char, mb_string, mb_string::size_type>(str, delim, keep_delim);
    }

    std::vector<wide_string> split_wide_string(const wide_string& str, wchar_t delim, bool keep_delim)
    {
        return _split_string_<wchar_t, wide_string, wide_string::size_type>(str, delim, keep_delim);
    }

    std::vector<mb_string> split_mb_string(const mb_string& str, const char* delims, bool keep_delims)
    {
        return _split_string_<char, mb_string, mb_string::size_type>(str, delims, keep_delims);
    }

    std::vector<wide_string> split_wide_string(const wide_string& str, const wchar_t* delims, bool keep_delims)
    {
        return _split_string_<wchar_t, wide_string, wide_string::size_type>(str, delims, keep_delims);
    }

    void mb_string_to_upper(mb_string& str)
    {
        _string_to_upper_<mb_string, char>(str);
    }

    void wide_string_to_upper(wide_string& str)
    {
        _string_to_upper_<wide_string, wchar_t>(str);
    }

    mb_string mb_string_as_upper(const mb_string& str)
    {
        return _string_as_upper_<mb_string, char>(str);
    }

    wide_string wide_string_as_upper(const wide_string& str)
    {
        return _string_as_upper_<wide_string, wchar_t>(str);
    }

    void mb_string_to_lower(mb_string& str)
    {
        _string_to_lower_<mb_string, char>(str);
    }

    void wide_string_to_lower(wide_string& str)
    {
        _string_to_lower_<wide_string, wchar_t>(str);
    }

    mb_string mb_string_as_lower(const mb_string& str)
    {
        return _string_as_lower_<mb_string, char>(str);
    }

    wide_string wide_string_as_lower(const wide_string& str)
    {
        return _string_as_lower_<wide_string, wchar_t>(str);
    }
}

#if AUL_MSVC
#pragma warning(pop)
#endif // AUL_MSVC