#include "process.h"
#include <memory>

int main(int argc, char *argv[])
{
    std::unique_ptr<Process> process_federate(new Process());
    process_federate ->  runFederate();

    return 0;
}
