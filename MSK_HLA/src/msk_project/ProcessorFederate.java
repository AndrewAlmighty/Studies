package msk_project;

import java.util.ArrayList;
import java.util.Random;
import hla.rti1516e.exceptions.RTIexception;
import hla.rti1516e.exceptions.RTIinternalError;


public class ProcessorFederate extends Federate
{	
    private int processors_number;
    private boolean processors_created = false;
    private ArrayList <Processor> processorList;

    public ProcessorFederate(int n)
    {
        processors_number = n;
        processorList = new ArrayList<Processor>();
    }

	@Override
	protected void onRun() throws RTIexception 
	{
            int i;

            if (!processors_created)
            {
                int proc_id = 0;
                while (proc_id < processors_number)
                {
                    processorList.add(new Processor(proc_id));
                    byte[] byte_id = Encoder.encodeInt(encoderFactory, proc_id);
                    this.sendInteraction("NewProcessor", "id", byte_id, 0);
                    proc_id++;
                }

                processors_created = true;
            }

            else
            {
                for (i = 0; i < processors_number; i++)
                {
                    if (processorList.get(i).getState() == 2)
                    {
                        log("Processor with:" + processorList.get(i).getID() + " finished work! Process id: " + processorList.get(i).getProcessID());
                        byte[] byte_processor_id = Encoder.encodeInt(encoderFactory, processorList.get(i).getID());
                        byte[] byte_process_id = Encoder.encodeInt(encoderFactory, processorList.get(i).getProcessID());
                        processorList.get(i).setState(Processor.State.Idle);
                        this.sendInteraction("JobFinished", "processor_id", byte_processor_id, "process_id",
                        byte_process_id, 0);
                        continue;
                    }
                }
            }
	}
	
	@Override
	protected void handleInteraction(Interaction e) throws RTIexception 
	{
            if (e.getName() == "DoJob")
            {
                int processor_id = e.getParameterInt(encoderFactory, "processor_id");
                int process_id = e.getParameterInt(encoderFactory, "process_id");

                for (int i = 0; i < processors_number; i++)
                {
                    if (processorList.get(i).getID() == processor_id)
                    {
                         log("Processor with:" + processor_id + " started work! - Process number:" + process_id);
                         processorList.get(i).startWork(process_id);
                         break;
                    }
                }
            }
	}
	
	@Override
	protected void publishAndSubscribe() throws RTIexception 
	{
            this.publishInteraction("NewProcessor");
            this.publishInteraction("JobFinished");
            this.subscribeToInteraction("DoJob", new String[] {"processor_id", "process_id", "process_type"});
	}
	
	public static void main( String[] args )
	{
                try
                {
                    int n = Integer.parseInt(args[0]);
                        ProcessorFederate federate = new ProcessorFederate(n);
                        ProcessorAmbassador ambassador = new ProcessorAmbassador(federate, 1.0);
                        federate.runFederate( "ProcessorFederate", "Processor", "MSK_FEDERATION", 1.0, ambassador);
                }
                catch( Exception rtie ) { rtie.printStackTrace(); }
	}
}
