#include "stdafx.h"

#include "environment.h"
#include "builder.h"

#include "xutil/compress/archive_reader.h"
#include "xutil/compress/archive_writer.h"

using namespace ship;

int main(int argc, char* argv[])
{
    RegisterExceptionTraps();

    ArchiveWriter writer("test.zip");
    bool a = writer.AddFile("a.exe", GetApplicationPath());
    bool b = writer.AddFile("b\\b.exe", GetApplicationPath());
    writer.Close();

    FILE* f = fopen("test.zip", "rb");
    fseek(f, 0, SEEK_END);
    size_t file_size = ftell(f);
    ArchiveReader reader(f, 0, file_size);


    bool rr = reader.ExtractFile("a.exe", "a.exe");
    fclose(f);

    logs::AddConsoleAppender(logs::app);
    logs::AddConsoleAppender(logs::build);

    YAML::Node config = YAML::LoadFile("..\\doc\\setup.yaml");
    cout << config["product"]["name"].as<string>() << endl;

    auto vars = make_shared<VarTable>();
    vars->set("src", "C:\\Documents");
    Builder builder(vars);
    builder.Build(config);
	return 0;
}
