#ifndef PROCESS_H_
#define PROCESS_H_

#include <string>

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

    Process(int PID, std::string UID);
    void runFederate() const;
	int getState() const;

private:
	int m_PID;
	std::string m_UID;
	State m_state;
};

#endif
