package msk_project;

public class main
{
	public static void main(String args[])
	{
		if (args.length() < 1)
		{
			System.out.println("Usage: java -jar process/process_list/processor");
			return;
		}
	}
	
	String mode = args[0];
	if (mode.equalsIgnoreCase("process")
		processFederate.main(args);
	else if (mode.equalsIgnoreCase("process_list")
		process_listFederate.main(args);
	else if (mode.equalsIgnoreCase("processor")
		processorFederate.main(args);
	else
		System.out.println("Error. Wrong federate name");
}