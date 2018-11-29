package msk_project;

import msk_project.Ambassador;
import msk_project.Federate;

public class ProcessAmbassador extends Ambassador
{
	public ProcessAmbassador(Federate federate, double lookahead) {
		super(federate, lookahead);
	}

}