#ifndef PROCESS_H_
#define PROCESS_H_

#include <string>
#include <memory>

#include "RTI.hh"

#define READY_TO_RUN L"ReadyToRun"

class Process
{
public:

	enum State
	{
		Running = 1,
		Waiting,
		Stopped,
		Zombie
	};

    Process();
//    void runFederate(std::wstring federateName);
//	int getState() const;

//	std::unique_ptr<RTIambassador> rtiAmb;
//	std::unique_ptr<ProcessAmbassador> fedAmb;

private:
	int m_PID;
	std::string m_UID;
	State m_state;
};

#endif
