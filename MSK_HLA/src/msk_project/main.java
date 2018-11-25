package msk_project;

public class main
{
	public static void main(String args[])
	{
		if (args.length < 1)
		{
			System.out.println("Usage: java -jar Process/ProcessList/Processor");
			return;
		}
		
	String mode = args[0];
	
	if (mode.equalsIgnoreCase("ProcessList"))
		ProcessListFederate.main(args);
		
	else
		System.out.println("Error. Wrong federate name");	
		
	}
}