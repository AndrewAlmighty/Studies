#include "processList.h"
#include <memory>

int main(int argc, char* argv[])
{
   std::unique_ptr<ProcessList> processListFederate (new ProcessList());
   processListFederate -> runFederate();

   return 0;
}
