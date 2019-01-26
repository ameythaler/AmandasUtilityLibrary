#include "stdafx.h"

#if AUL_WIN

#include <windows.h>

#elif AUL_POSIX

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "aul/string.h"

#endif // AUL_WIN

namespace aul
{
    bool start_process(const char* procname, char* cmdline)
    {
#if AUL_WIN
        STARTUPINFOA info;
        PROCESS_INFORMATION proc_info;

        memset(&info, 0, sizeof(info));
        info.cb = sizeof(info);
        memset(&proc_info, 0, sizeof(proc_info));

        bool status = CreateProcessA(nullptr, cmdline, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &info, &proc_info);

        CloseHandle(proc_info.hProcess);
        CloseHandle(proc_info.hThread);

#elif AUL_POSIX

        bool status = true;
        pid_t pid = vfork();
        if (pid == 0) // child
        {
            mb_string cmdstr(cmdline);
            auto vec = split_mb_string(cmdstr, " ", false);
            char** args = new char*[vec.size() + 2];
            size_t i = 0;
            args[i++] = procname;
            for (mb_string& cmdarg : vec)
            {
                args[i++] = &cmdarg[0];
            }
            args[i] = nullptr;

            execcvp(procname, args);

            delete[] args;
            exit(1);
        }
        else if (pid < 0)
        {
            status = false;
        }

#endif // AUL_WIN

        return status;
    }
}