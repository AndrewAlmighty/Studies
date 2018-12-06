package msk_project;

import hla.rti1516e.exceptions.RTIexception;
import hla.rti1516e.exceptions.RTIinternalError;

import java.util.Queue; 
import java.util.LinkedList;
import java.util.HashMap;
import java.util.Map;
import java.util.Iterator;
import java.util.Set;

public class ProcessListFederate extends Federate
{
        private Queue<Process> processList;
	private HashMap<Integer, Boolean> processorsList;
	
	private int processes_in;
	private int processes_out;
	
	public ProcessListFederate()
	{
                processList = new LinkedList<Process>();
		processorsList = new HashMap<Integer, Boolean>();	//<ID, isFree>
		processes_in = 0;
		processes_out = 0;
	}
	
	@Override
	protected void onRun() throws RTIexception 
	{
		Set set = processorsList.entrySet();
		Iterator iterator = set.iterator();
		while(iterator.hasNext()) 
		{
			Map.Entry mentry = (Map.Entry)iterator.next();
        	if ((Boolean)mentry.getValue() == true)
			{
				if (processList.peek() != null)
				{
                                        Process task = processList.poll();
					int processor_id = (Integer)mentry.getKey();
					log("Processor with ID: " + processor_id + " begins to execute process with" + 
                                        "ID: " + task.getID());
					processorsList.put(processor_id, false);
					
					try 
					{
						byte[] byte_processor_id = Encoder.encodeInt(encoderFactory, processor_id);
                                                byte[] byte_process_id = Encoder.encodeInt(encoderFactory, task.getID());
                                                byte[] byte_process_type = Encoder.encodeInt(encoderFactory, task.getType());
						this.sendInteraction("DoJob", "processor_id", byte_processor_id, "process_id",
                                                byte_process_id, "process_type", byte_process_type, 0);
						processes_out++;
					} 	
					catch (RTIinternalError e) 
					{
						e.printStackTrace();
					}
				}
			}
      	}
	}
	
	@Override
	protected void handleInteraction(Interaction e) throws RTIexception 
	{
		if (e.getName() == "JobFinished")
		{
			int processor_id = e.getParameterInt(encoderFactory, "processor_id");
			processorsList.put(processor_id, true);
		}
		
		else if (e.getName() == "NewProcess")
		{
			int id = e.getParameterInt(encoderFactory, "id");
                        int type = e.getParameterInt(encoderFactory, "type");
                        processList.add(new Process(id, type));
			log("New process in queue. ID:" + id);
			processes_in++;
		}
		
		else if (e.getName() == "NewProcessor")
		{
			int id = e.getParameterInt(encoderFactory, "id");
			processorsList.put(id, true);
		}
	}
	
	@Override
	protected void publishAndSubscribe() throws RTIexception 
	{
		this.publishInteraction("DoJob");
		this.subscribeToInteraction("JobFinished", new String[] {"processor_id", "process_id"});
		this.subscribeToInteraction("NewProcess", new String[] {"id", "type", "state"});
		this.subscribeToInteraction("NewProcessor", new String[] {"id"});
	}
	
	public static void main( String[] args )
	{
		try
		{
			ProcessListFederate federate = new ProcessListFederate();
			ProcessListAmbassador ambassador = new ProcessListAmbassador(federate, 1.0);
			federate.runFederate( "ProcessListFederate", "ProcessList", "MSK_FEDERATION", 1.0, ambassador);
		}
		catch( Exception rtie ) { rtie.printStackTrace(); }
	}
}
