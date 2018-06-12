#include "process.h"
#include "../StringHelpers.h"
#include <memory>
#include <string>

int main(int argc, char *argv[])
{
	std::wstring federateName = L"exampleProcess";

	if(argc > 1)
		federateName = toWideString(std::string(argv[1]));

    std::unique_ptr<Process> process_federate(new Process());
    process_federate ->  runFederate(federateName);

    return 0;
}
