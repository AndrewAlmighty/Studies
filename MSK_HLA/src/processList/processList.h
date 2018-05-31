#ifndef PROCESSLIST_H_
#define PROCESSLIST_H_

#include <list>

class ProcessList
{

public:
    ProcessList();
    void runFederate();

private:
	std::list<int> m_queue;
};

#endif
