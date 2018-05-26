#ifndef PROCESSOR_H_
#define PROCESSOR_H_

class Processor
{
public:

	enum State
	{
		working,
		free,
		unavailable
	};

    Processor(int ID);
    void runFederate();

private:
	int m_ID;
	State m_state;
};

#endif
