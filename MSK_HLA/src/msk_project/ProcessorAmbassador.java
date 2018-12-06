package msk_project;

import msk_project.Ambassador;
import msk_project.Federate;

public class ProcessorAmbassador extends Ambassador
{
        public ProcessorAmbassador(Federate federate, double lookahead) {
		super(federate, lookahead);
	}

}
