#include "stdafx.h"
#include "exception_formatter.h"
#include "../system/path.h"

#include "../regex.h"
#include "../strutil.h"

#include "exception_traps.h"
#include "symbol_server.h"

namespace xutil
{
    namespace 
    {
        Regex exp_throw("(.*): Throw in function (.*)");

        string ExtractExceptionType(const type_info& type)
        {
            string type_name = type.name();
            int space_pos = type_name .find_last_of(' ');
            if (space_pos >= 0)
            {
                type_name = type_name.substr(space_pos + 1);
            }
            return type_name;
        }

        Regex exp_function("([^ ]*)\\(");
        string ExtractFunctionName(const string& full_name)
        {
            MatchResults what;
            if (regex_search(full_name, what, exp_function))
            {
                return what[1];
            }

            return full_name;
        }

        Regex exp_exception_type("clone_impl<[^>]* ([^ >]*)>");
        string ExtractExceptionType(const string& info)
        {
            MatchResults what;
            if (regex_search(info, what, exp_exception_type))
            {
                return what[1];
            }

            return info;
        }

        Regex exp_exception_detail("^\\[struct .*_(.*)_ \\*\\] = (.*)");
        bool ExtractErrorDetails(const string& info, string* name, string* value)
        {
            MatchResults what;
            if (regex_search(info, what, exp_exception_detail))
            {
                *name = what[1];
                *value = what[2];

                replace_all(*name, "_", " ");
                return true;
            }

            return false;
        }

        void PrintStringVector(stringstream& stream, const string& key, const vector<string>& strings)
        {
            stream << "  " << key << ": ";
            switch (strings.size())
            {
            case 0:
                stream << endl;
                break;
            case 1:
                stream << strings[0] << endl;
                break;
            default:
                stream << endl;
                for (size_t i = 0; i < strings.size(); ++i)
                {
                    stream << "    " << strings[i] << endl;
                }
                break;
            }
        }
    }

    string format_exception(const std::exception& e, bool extract_stack_trace /* = true */)
    {
        string exception_type = ExtractExceptionType(typeid(e));

        stringstream res;
        res << "Exception of type " << exception_type << endl;
        res << "  Message: " << e.what() << endl;

        if (extract_stack_trace)
        {
            PEXCEPTION_POINTERS ex_info = GetExceptionStackTracePointers(e);
            if (ex_info)
            {
                BacktraceStack stack = SymbolServer::instance()->DumpContext(ex_info->ContextRecord);
                PrintStringVector(res, "Stack trace", stack);
            }
        }

        return res.str();
    }

    string format_exception(const boost::exception& e, bool extract_stack_trace /* = true */)
    {
        return format_boost_exception_details(diagnostic_information_what(e));
    }

    string format_exception(const Exception& e)
    {
        return e.what();
    }

    string format_boost_exception_details(const string& boost_details)
    {
        try
        {
            string file_name;
            string function_name;
            string exception_type;
            map<string, vector<string>> details;

            vector<string> lines = split(boost_details, "\r\n");
            MatchResults what;
            if (lines.size() > 1)
            {
                if (regex_search(lines[0], what, exp_throw))
                {
                    string file_path = what[1];
                    string function = what[2];

                    file_name = ExtractFileName(file_path);
                    function_name = ExtractFunctionName(function);
                }
                else
                {
                    function_name = lines[0];
                }

                exception_type = ExtractExceptionType(lines[1]);
                int x = 1;
            }

            auto cur_detail = details.end();
            for (size_t i = 2; i < lines.size(); ++i)
            {
                const string& line = lines[i];
                string name;
                string value;
                if (ExtractErrorDetails(line, &name, &value))
                {
                    cur_detail = details.insert(make_pair(name, vector<string>())).first;
                    cur_detail->second.push_back(value);
                }
                else if (cur_detail != details.end() && !line.empty())
                {
                    cur_detail->second.push_back(line);
                }
            }

            stringstream stream;
            stream << "Exception of type " << exception_type << endl;
            stream << "  Thrown at " << function_name;
            if (!file_name.empty())
            {
                stream << "   " << file_name;
            }
            stream << endl;

            auto i_det = details.find("Message");
            if (i_det != details.end())
            {
                PrintStringVector(stream, i_det->first, i_det->second);
            }

            for (i_det = begin(details); i_det != end(details); ++i_det)
            {
                if (i_det->first != "Message" && i_det->first != "Backtrace")
                {
                    PrintStringVector(stream, i_det->first, i_det->second);
                }
            }

            i_det = details.find("Backtrace");
            if (i_det != details.end())
            {
                PrintStringVector(stream, "Stack trace", i_det->second);
            }

            return stream.str();
        }
        catch (...)
        {
        }
        //ITS_CATCH_EXCEPTION(logs::app, "Error while formatting exception details")
        return boost_details;
    }
}