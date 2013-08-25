#include "stdafx.h"

#include "environment.h"
#include "builder.h"

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

    logs::AddConsoleAppender(logs::app);
    logs::AddConsoleAppender(logs::build);

    try
    {
        foo();
    }
    CATCH_EXCEPTION(logs::build, "Build error")


    YAML::Node config = YAML::LoadFile("..\\doc\\setup.yaml");
    cout << config["product"]["name"].as<string>() << endl;

    auto vars = make_shared<VarTable>();
    vars->set("src", "C:\\Documents");
    Builder builder(vars);
    builder.Build(config);
	return 0;
}
