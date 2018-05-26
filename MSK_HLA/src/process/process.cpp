#include "process.h"
#include <iostream>

Process::Process(int PID, std::string UID)
{
	m_PID = PID;
	m_UID = UID;
	m_state = State::Waiting;
}

void Process::runFederate() const
{
    std::cout << "Process PID: " << m_PID << " UID: " << m_UID << " state: " << getState() << std::endl;
}

int Process::getState() const
{
	return m_state;
}


