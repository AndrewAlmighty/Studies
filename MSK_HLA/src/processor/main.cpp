#include "processor.h"
#include <memory>

int main(int argc, char* argv[])
{
    std::unique_ptr<Processor> processor_federate (new Processor());
    processor_federate -> runFederate();

    return 0;
}
