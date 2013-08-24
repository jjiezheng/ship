#include "stdafx.h"

#include "environment.h"
#include "builder.h"

#include "xutil/debug/stack_walker.h"

using namespace ship;


void OutputProc(const char* text)
{
    cerr << text;
    cerr.flush();
}

LONG CALLBACK VectoredHandler(PEXCEPTION_POINTERS ExceptionInfo)
{
    StackWalker walker;
    walker.SetOutputProc(&OutputProc);
    walker.ShowCallstack(::GetCurrentThread(), ExceptionInfo->ContextRecord);
    return EXCEPTION_CONTINUE_SEARCH;
}

void bar()
{
    throw exception("test");
}

void foo()
{
    bar();
}

int main(int argc, char* argv[])
{
    //AddVectoredExceptionHandler(1, &VectoredHandler);

    try
    {
        foo();
    }
    catch (exception& e)
    {
        void* pe = &e;
        typeid(*pe).name();
        cout << e.what() << endl;
    }

    logs::AddConsoleAppender(logs::build);

    YAML::Node config = YAML::LoadFile("C:\\Documents\\Other\\ship\\doc\\setup.yaml");
    cout << config["product"]["name"].as<string>() << endl;

    auto vars = make_shared<VarTable>();
    vars->set("src", "C:\\Documents");
    Builder builder(vars);
    builder.Build(config);
	return 0;
}
