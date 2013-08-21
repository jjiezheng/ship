#include "stdafx.h"

using namespace ship;

int main(int argc, char* argv[])
{
    YAML::Node config = YAML::LoadFile("C:\\Documents\\Other\\ship\\doc\\setup.yaml");
    cout << config["product"]["name"].as<string>() << endl;
    YAML::Node files = config["files2"];
    cout << config["product"].IsDefined() << endl;
    cout << files.IsDefined() << endl;
    for (auto i = files.begin(); i != files.end(); ++i)
    {
        cout << i->first.as<string>() << endl;
    }
	return 0;
}

