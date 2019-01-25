#include "stdafx.h"
#include "aul/date_time.h"
#include <time.h>

#if AUL_MSVC
#pragma warning(push)
#pragma warning(disable: 4996) // disable unbounded time function deprecation warnings
#endif // AUL_MSVC

namespace aul
{
    aul_string date_time::get_date_string()
    {
        time_t cur_time = time(0);
        tm* local_time = localtime(&cur_time);
        aul_stringstream time_stream;
        time_stream << local_time->tm_mon + 1 << '/' << local_time->tm_mday << '/' << local_time->tm_year + 1900;
        return time_stream.str();
    }

    aul_string date_time::get_time_string()
    {
        time_t cur_time = time(0);
        tm* local_time = localtime(&cur_time);
        aul_stringstream time_stream;
        time_stream << local_time->tm_hour << ':' << local_time->tm_min << ':' << local_time->tm_sec;
        return time_stream.str();
    }
}

#if AUL_MSVC
#pragma warning(pop)
#endif // AUL_MSVC