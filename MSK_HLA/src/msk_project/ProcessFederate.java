package msk_project;

import java.util.Random;
import hla.rti1516e.exceptions.RTIexception;
import hla.rti1516e.exceptions.RTIinternalError;


public class ProcessFederate extends Federate
{
    int idCounter = 0;
    int wait = 0;

	public ProcessFederate()
	{

	}
	
	@Override
	protected void onRun() throws RTIexception 
	{
            if (wait < 10)
                wait++;

            else
            {
                if (new Random().nextInt(2) == 1)
		{                    
                    log("Created new process:" + idCounter);
                    int type = new Random().nextInt(3);
                    byte[] byte_process_id = Encoder.encodeInt(encoderFactory, idCounter);
                    byte[] byte_type = Encoder.encodeInt(encoderFactory, type);
                    byte[] byte_state = Encoder.encodeInt(encoderFactory, 0); //state - idle
                    this.sendInteraction("NewProcess", "id", byte_process_id, "type", byte_type, "state", byte_state, 0);
                    idCounter++;
                }
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
