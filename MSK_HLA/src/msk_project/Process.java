package msk_project;

public class Process
{
	public enum State
	{
		Idle, Executed	
	};
	
	private static int IDCounter = 0;
	private int id = -1;
	private State m_state = State.Idle;
	
	public Process()
	{
		id = IDCounter;
		IDCounter++;
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
}