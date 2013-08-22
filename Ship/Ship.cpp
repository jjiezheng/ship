#include "stdafx.h"

#include "environment.h"
#include "builder.h"

using namespace ship;

int main(int argc, char* argv[])
{
    logs::AddConsoleAppender(logs::build);

    YAML::Node config = YAML::LoadFile("C:\\Documents\\Other\\ship\\doc\\setup.yaml");
    cout << config["product"]["name"].as<string>() << endl;

    auto vars = make_shared<VarTable>();
    vars->set("src", "C:\\Documents");
    Builder builder(vars);
    builder.Build(config);
	return 0;
}

