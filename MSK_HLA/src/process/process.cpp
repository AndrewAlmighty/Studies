#include "process.h"
#include <iostream>

Process::Process()
{
	m_PID = 10;
	m_UID = "root";
	m_state = State::Waiting;
}

void Process::runFederate(std::wstring federateName)
{
	//1. Create the RTIambassador
	RTIambassadorFactory factory = RTIambassadorFactory();
	this -> rtiAmb = factory.createRTIambassador().release();

	//2. Connect to the RTI.
	this -> fedAmb = new ProcessAmbassador();
	
	try
	{
		rtiAmb -> connect(*this -> mfedAmb, HLA_EVOKED);
	}

	catch(ConnectionFailed& connectionFailed)
	{
		std::wcout << L"Connection failed: " << connectionFailed.what() << std::endl;
	}

	catch(InvalidLocalSettingDesignator& settings)
	{
		std::wcout << L"Connection failed, InvalidLocalSettingsDesignator: " << settings.what() << std::endl;
	}

	catch(UnsupportedCallBackModel& model)
	{
		std::wcout << L"Connection failed, UnsupportedCallbackModel: " << model.what() << std::endl;
	}

	catch(AlreadyConnected& connected)
	{
		std::wcout << L"ConnectionFailed, AlreadyConnected: " << connected.what() << std::endl;
	}

	catch(RTIinternalError& error)
	{
		wcout << L"Connection failed, Generic Error: " << error.what() << std::endl;
	}

	//3. Create the federation
	try
	{
		std::vector<wstring> foms;
		foms.push_back(L"MyFom.fed");
		rtiAmb -> createFederationExecution(L"MyFederation", foms);
		std::wcout << L"Created federation!" << std::endl;
	}

	catch(FederationExecutionAlreadyExists& exists)

		std::wcout << L"Federation already existed!" << std::endl;
	}

	//4. Join the federation
	rtiAmb -> joinFederationExecution(federateName, L"Process Federate" ,L"MyFederation");
	std::wcout << L"Joined Federation as " << federateName << std::endl;
	
	//Initialize the handles - have to wait until we are joined
	initializeHandles();

	//5. Announce the sync point
	VariableLengthData tag((void*) "", 1);
	rtiAmb -> registerFederationSynchronizationPoint(READY_TO_RUN, tag);

	while(fedAmb -> isReadyToRun == false)
		rtiAmb -> evokeMultipleCallbacks(0.1, 1.0);

	//waitForUser - we wait until ENTER is hit. This is time to run other federates.
	waitForUser();

	//6. Achieve the point and wait for synchronization
	rtiAmb -> synchronizationPointAcheved(READY_TO_RUN);
	std::wcout << L"Achieved sync point: " << READY_TO_RUN << L", waiting for federation ..." << std::endl;
	
	while(fedAmb -> isReadyToRun == false)
		rtiAmb -> evokeMultipleCallbacks(0.1, 1.0);

	//7. enable policies
	enableTimePolicy();
	std::wcout << L"Time Policy Enabled!" << std::endl;

	//8. Publish and subscribe
	publishAndSubscribe();
	std::wcout << L"Published and Subscribed" << std::endl;

	//9. Register an object to update
	ObjectInstanceHandle objectHandle = registerObject();
	std::cout << L"Registered Object, handle=" << objectHandle << endl;
	
	//10. Simulation loop

	for(int i = 0; i < 20; i++)
	{
		updateAttributeValues(objectHandle);
		sendInteraction();
		advanceTime(1.0);
		wcout << L"Time advanced to " << fedAmb -> federateTime << std::endl;
	}

	//11. Delete object
	deleteObject(objectHandle);
	std::wcout << L"Object deleted, handle = " << objectHandle << std::endl;

	//12. Resign from the federation
	rtiAmb -> resignFederationExecution(NO_ACTION);
	std::wcout << L"Resigned from federation" << std::endl;

	//13 Try to destroy federation
	try
	{
		rtiAmb -> destroyFederationExecution(L"MyFederation");
		std::wcout << L"Federation destroyed" << std::endl;
	}

	catch(FederationExecutionDoesNotExist& error)
	{
		std::wcout << L"Federation already destroyed" << std::endl;
	}

	catch(FederatesCurrentlyJoined& error)
	{
		std::wcout << L"Federates still joined!" << std::endl;
	}

	this -> rtiAmb -> disconnect();
}

void Process::waitForUser()
{
	std::wcout << L" >>>>>>> Press enter to continue <<<<<<<" << std::endl;
	std::string line;
	std::getline(cin, line);
}

int Process::getState() const
{
	return m_state;
}



