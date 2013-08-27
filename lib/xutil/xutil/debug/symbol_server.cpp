#include "stdafx.h"
#include "symbol_server.h"

#include <dbghelp.h>
#include <boost/format.hpp>
#include "../threading/primitives.h"
#include "../system/path.h"

namespace xutil
{
    namespace
    {
        SymbolServer* g__instance_ = 0;
        Mutex g__symbol_server_mutex_;

        string FormatFrameInfo(DWORD64 frame, DWORD64 displacement, PSYMBOL_INFO symbol, PIMAGEHLP_LINE64 line)
        {
            string str_frame;
            if (symbol) 
            {
                str_frame += str(format("%1% [%2%+%3%]") % symbol->Name % io::group(hex, uppercase, frame) % displacement);
            } 
            else 
            {
                str_frame += str(format("(No symbol) [%1%]") % io::group(hex, uppercase, frame));
            }

            if (line)
            {
                str_frame += str(format(" (%1%:%2%)") % ExtractFileName(line->FileName) % line->LineNumber);
            }
            return str_frame;
        }

        string FormatFrameInfo(HANDLE process, DWORD64 frame, string* symbol_name = 0)
        {
            const int kMaxNameLength = 256;
            char buffer[sizeof(SYMBOL_INFO) + kMaxNameLength * sizeof(wchar_t)];
            memset(buffer, 0, sizeof(buffer));

            DWORD64 displacement = 0;
            PSYMBOL_INFO symbol = reinterpret_cast<PSYMBOL_INFO>(&buffer[0]);
            symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
            symbol->MaxNameLen = kMaxNameLength - 1;
            BOOL has_symbol = SymFromAddr(process, frame, &displacement, symbol);

            DWORD line_displacement = 0;
            IMAGEHLP_LINE64 line = {};
            line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
            BOOL has_line = SymGetLineFromAddr64(process, frame, &line_displacement, &line);

            if (symbol_name)
            {
                if (has_symbol)
                {
                    symbol_name->assign(symbol->Name);
                }
                else
                {
                    symbol_name->clear();
                }
            }
            return FormatFrameInfo(frame, displacement, has_symbol ? symbol : NULL, has_line ? &line : NULL);
        }
    }

    SymbolServer* SymbolServer::instance()
    {
        if (!g__instance_)
        {
            UniqueLock lock(g__symbol_server_mutex_);
            if (!g__instance_)
            {
                g__instance_ = new SymbolServer();
            }
        }
        return g__instance_;
    }

    DWORD SymbolServer::init_error() const 
    {
        return init_error_; 
    }

    SymbolServer::SymbolServer() 
        : init_error_(ERROR_SUCCESS) 
    {
        filter_stubs = true;
        stack_startup_entries_.push_back("__tmainCRTStartup");
        stack_throw_entries_.push_back("CxxThrowException");
        stack_throw_entries_.push_back("RaiseException");

        DWORD options = SymGetOptions();
        options |= SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_LOAD_LINES;
        SymSetOptions(options);

        if (!SymInitialize(GetCurrentProcess(), NULL, TRUE)) 
        {
            init_error_ = GetLastError();
        }
    }

    bool SymbolServer::DumpContext(BacktraceStack* stack, PCONTEXT context, HANDLE thread /* = 0 */, int skip /* = 0 */)
    {
        if (init_error_ != ERROR_SUCCESS)
        {
            return false;
        }

        UniqueLock lock(g__symbol_server_mutex_);

        STACKFRAME64 sf = {};
        sf.AddrPC.Offset    = context->Eip;
        sf.AddrPC.Mode      = AddrModeFlat;
        sf.AddrStack.Offset = context->Esp;
        sf.AddrStack.Mode   = AddrModeFlat;
        sf.AddrFrame.Offset = context->Ebp;
        sf.AddrFrame.Mode   = AddrModeFlat;

        HANDLE process = GetCurrentProcess();
        if (!thread)
        {
            thread = GetCurrentThread();
        }

        for (;;) 
        {
            SetLastError(0);
            BOOL stack_walk_ok = StackWalk64(IMAGE_FILE_MACHINE_I386, process, thread, &sf, context, 0, &SymFunctionTableAccess64, &SymGetModuleBase64, 0);
            if (!stack_walk_ok || !sf.AddrFrame.Offset)
            {
                break;
            }

            if (skip) 
            {
                --skip;
            }
            else 
            {
                DWORD64 frame = sf.AddrPC.Offset;

                string symbol_name;
                string str_frame = FormatFrameInfo(process, frame, &symbol_name);

                if (filter_stubs)
                {
                    if (find(stack_startup_entries_.begin(), stack_startup_entries_.end(), symbol_name) != stack_startup_entries_.end())
                    {
                        break;
                    }
                    else if (find(stack_throw_entries_.begin(), stack_throw_entries_.end(), symbol_name) != stack_throw_entries_.end())
                    {
                        continue;
                    }
                }

                stack->push_back(str_frame);
            }
        }

        return true;
    }

    BacktraceStack SymbolServer::DumpContext(PCONTEXT context, HANDLE thread /* = 0 */, int skip /* = 0 */)
    {
        BacktraceStack stack;
        DumpContext(&stack, context, thread);
        return stack;
    }

    bool SymbolServer::DumpTrace(BacktraceStack* stack, const void* const* trace,  int count)
    {
        if (init_error_ != ERROR_SUCCESS)
        {
            return false;
        }

        UniqueLock lock(g__symbol_server_mutex_);

        HANDLE process = GetCurrentProcess();
        for (int i = 0; i < count; ++i) 
        {
            DWORD_PTR frame = reinterpret_cast<DWORD_PTR>(trace[i]);

            string symbol_name;
            string str_frame = FormatFrameInfo(process, frame, &symbol_name);

            if (filter_stubs)
            {
                if (find(stack_startup_entries_.begin(), stack_startup_entries_.end(), symbol_name) != stack_startup_entries_.end())
                {
                    break;
                }
                else if (find(stack_throw_entries_.begin(), stack_throw_entries_.end(), symbol_name) != stack_throw_entries_.end())
                {
                    continue;
                }
            }

            stack->push_back(str_frame);
        }

        return true;
    }

    BacktraceStack SymbolServer::DumpTrace(const void* const* trace,  int count)
    {
        BacktraceStack stack;
        DumpTrace(&stack, trace, count);
        return stack;
    }
}