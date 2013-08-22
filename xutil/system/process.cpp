#include "stdafx.h"
#include "process.h"
#include "path.h"

#ifdef WIN32
    #include <shellapi.h>
    #include <tlhelp32.h>
#endif

namespace xutil
{
#ifdef WIN32
    void RestartApplication(int secs_to_wait /* = 2 */, string working_dir /* = "" */, void (*exit_function)() /* = nullptr */)
    {
        string app_path = GetApplicationPath();
        string shell = "cmd.exe";
        string command = str(format("/c ping -n %1% 127.0.0.1 & start %2%") % secs_to_wait % app_path);

        if (working_dir.empty())
        {
            working_dir = ExtractParentDirectory(app_path);
        }

        if (!ShellExecute(NULL, NULL, shell.c_str(), command.c_str(), working_dir.c_str(), SW_HIDE))
        {
            throw SystemError(format("Failed to initiate application restart command: %1% %2%") % shell % command);
        }

        if (exit_function)
        {
            exit_function();
        }
        else
        {
            ExitApplication();
        }
    }
#endif

    void ExitApplication(int exit_code /* = -1 */)
    {
        exit(exit_code);
    }

    unsigned GetApplicationProcessID()
    {
        return GetCurrentProcessId();
    }

    void SuspendApplicationThreads()
    {
        HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (h == INVALID_HANDLE_VALUE) 
        {
            return;
        }

        THREADENTRY32 te;
        te.dwSize = sizeof(te);
        if (Thread32First(h, &te)) 
        {
            DWORD pid = GetCurrentProcessId();
            DWORD tid = GetCurrentThreadId();

            do 
            {
                if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(te.th32OwnerProcessID)) 
                {
                    if (te.th32OwnerProcessID == pid)
                    {
                        if (te.th32ThreadID != tid)
                        {
                            HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);
                            if (hThread)
                            {
                                SuspendThread(hThread);
                                CloseHandle(hThread);
                            }
                        }
                    }
                }
                te.dwSize = sizeof(te);
            } 
            while (Thread32Next(h, &te));
        }
        CloseHandle(h);
    }
}