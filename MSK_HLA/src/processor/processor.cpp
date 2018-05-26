#include "processor.h"
#include <iostream>

Processor::Processor(int ID)
{
	m_ID = ID;
	m_state = free;
}

void Processor::runFederate()
{
    std::cout << "Processor ID:" << m_ID << " state:" << m_state;
}

