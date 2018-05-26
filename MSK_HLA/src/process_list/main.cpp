#include "process_list.h"
#include <memory>

int main(int argc, char* argv[])
{
   std::unique_ptr<Process_list> process_list_federate (new Process_list());
   process_list_federate -> runFederate();

   return 0;
}
