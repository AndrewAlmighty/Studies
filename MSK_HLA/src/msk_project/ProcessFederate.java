package msk_project;

import hla.rti1516e.exceptions.RTIexception;
import hla.rti1516e.exceptions.RTIinternalError;


public class ProcessFederate extends Federate
{
	Process m_process;
	int a;
	
	public ProcessFederate()
	{
		a = 0;
	}
	
	@Override
	protected void onRun() throws RTIexception 
	{
		if(a == 0)
		{
		m_process = new Process();
		byte[] byte_process_id = Encoder.encodeInt(encoderFactory, m_process.getID());
		byte[] byte_type = Encoder.encodeInt(encoderFactory, 4);
		byte[] byte_state = Encoder.encodeInt(encoderFactory, m_process.getState());
		this.sendInteraction("NewProcess", "id", byte_process_id, "type",
						byte_type, "state", byte_state,0);
						a++;
						}
						
	}
	
	@Override
	protected void handleInteraction(Interaction e) throws RTIexception 
	{

	}
	
	@Override
	protected void publishAndSubscribe() throws RTIexception 
	{
		this.publishInteraction("NewProcess");
		this.subscribeToInteraction("DoJob", new String[] {"processor_id", "process_id"});
	}
	
	public static void main( String[] args )
	{
		try
		{
			ProcessFederate federate = new ProcessFederate();
			ProcessAmbassador ambassador = new ProcessAmbassador(federate, 1.0);
			federate.runFederate( "ProcessFederate", "Process", "MSK_FEDERATION", 1.0, ambassador);
		}
		catch( Exception rtie ) { rtie.printStackTrace(); }
	}
}