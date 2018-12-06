package msk_project;

public class Process
{
	public enum State
	{
		Idle, Executed	
	};
	
	private int id = -1;
        private int type = 0;   //user - 1, system - 2
	private State m_state = State.Idle;
	
        public Process(int di, int t)
	{
                id = di;
                type = t;
	}
	
	public void setState(State s)
	{
		m_state = s;
	}
	
	public int getState()
	{
		return m_state.ordinal();	
	}
	
	public int getID()
	{
		return id;
	}

        public int getType()
        {
            return type;
        }

        public void setType(int t)
        {
            type = t;
        }

}
