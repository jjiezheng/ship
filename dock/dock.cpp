#include "stdafx.h"

#include "libship/environment.h"
#include "libship/builder.h"
#include "libship/package_writer.h"

#include "xutil/compress/archive_reader.h"
#include "xutil/compress/archive_writer.h"

using namespace ship;

int main(int argc, char* argv[])
{
    cout << CONFIGURATION_NAME << endl;
    RegisterExceptionTraps();

    logs::AddConsoleAppender(logs::app);
    logs::AddConsoleAppender(logs::build);

    YAML::Node config = YAML::LoadFile("..\\..\\..\\doc\\setup.yaml");
    cout << config["product"]["name"].as<string>() << endl;

    auto vars = make_shared<VarTable>();
    vars->set("src", "C:\\Documents");
    Builder builder(vars);
    PackagePtr& package = builder.Build(config);

    PackageWriter writer("..\\..\\stub\\" CONFIGURATION_NAME "\\stub.exe");
    writer.Compose("setup.exe", package);

	return 0;
}

