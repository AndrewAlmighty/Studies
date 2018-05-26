#ifndef PROCESS_LIST_H_
#define PROCESS_LIST_H_

#include <list>

class Process_list
{

public:
    Process_list();
    void runFederate();

private:
	std::list<int> m_queue;
};

#endif
