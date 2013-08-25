#include "stdafx.h"

#include "environment.h"
#include "builder.h"

#include "xutil/debug/untyped_exception.h"
#include "xutil/debug/symbol_server.h"

using namespace ship;


void bar()
{
    throw runtime_error("test");
}

void foo()
{
    bar();
}

int main(int argc, char* argv[])
{
    RegisterExceptionTraps();
    try
    {
        foo();
    }
    catch (exception& e)
    {
        cerr << format_exception(e);
    }

    logs::AddConsoleAppender(logs::build);

    YAML::Node config = YAML::LoadFile("..\\doc\\setup.yaml");
    cout << config["product"]["name"].as<string>() << endl;

    auto vars = make_shared<VarTable>();
    vars->set("src", "C:\\Documents");
    Builder builder(vars);
    builder.Build(config);
	return 0;
}
