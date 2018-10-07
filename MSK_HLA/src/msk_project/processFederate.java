package msk_project;

import hla.rti1516e.AttributeHandle;
import hla.rti1516e.AttributeHandleSet;
import hla.rti1516e.AttributeHandleValueMap;
import hla.rti1516e.CallbackModel;
import hla.rti1516e.ObjectClassHandle;
import hla.rti1516e.ObjectInstanceHandle;
import hla.rti1516e.RTIambassador;
import hla.rti1516e.ResignAction;
import hla.rti1516e.RtiFactoryFactory;
import hla.rti1516e.encoding.EncoderFactory;
import hla.rti1516e.encoding.HLAinteger32BE;
import hla.rti1516e.exceptions.FederatesCurrentlyJoined;
import hla.rti1516e.exceptions.FederationExecutionAlreadyExists;
import hla.rti1516e.exceptions.FederationExecutionDoesNotExist;
import hla.rti1516e.exceptions.RTIexception;
import hla.rti1516e.time.HLAfloat64Interval;
import hla.rti1516e.time.HLAfloat64Time;
import hla.rti1516e.time.HLAfloat64TimeFactory;

public class processFederate
{
    private RTIambassador rtiamb;
    private processFederateAmbassador fedamb;
    private HLAfloat64TimeFactory timeFactory;
    private String federateName;

    protected EncoderFactory encoderFactory;
    protected ObjectClassHandle processHandle;
    protected ObjectClassHandle process_listHandle;
    protected ObjectClassHandle processorHandle;
    protected AttributeHandle processIdHandle;
    protected AttributeHandle processTypeHandle;
    protected AttributeHandle processStateHandle;

    private int id = -1;
    private int state = 0;  //0 - idle, 1 - being processed, 2 - finished
    private int type = 0;   //0 - normal, 1 - high priority

    private void log(String message)
    {
        System.out.println(this.federateName + " with ID: " + id + " says: " + message);
    }

    private void waitForUser()
    {
        this.log( ">>>>>>>>>> Press Enter to Continue <<<<<<<<<<" );
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        try
        {
                reader.readLine();
        }
        catch(Exception e)
        {
                this.log("Error while waiting for user input: " + e.getMessage());
                e.printStackTrace();
        }
    }

    public void runFederate(String federateName) throws Exception
    {
        this.federateName = federateName;
        this.log("Creating RTI Ambassador...");
        this.rtiamb = RtiFactoryFactory.getRtiFactory().getRtiAmbassador();
        this.encoderFactory = RtiFactoryFactory.getRtiFactory().getEncoderFactory();

        this.log("Connecting...");
        this.fedamb = new processFederateAmbassador(this, "processFederateAmbassador");
        this.rtiamb.connect(fedamb, CallbackModel.HLA_EVOKED);

        try {
                File fom = new File( "msk_fom.fed" );
                this.rtiamb.createFederationExecution("MskFederation", modules);
                this.log("Created Federation.");
        }
        catch(FederationExecutionAlreadyExists exists)
        {
                this.log("Didn't create federation, it already existed.");
        }
        catch(MalformedURLException urle) {
                this.log("Exception loading one of the FOM modules from disk: " + urle.getMessage());
                urle.printStackTrace();
                return;
        }

        this.rtiamb.joinFederationExecution(this.federateName, this.federateName + "Type", "MskFederation");
        this.log("Joined Federation as " + this.federateName);

        this.timeFactory = (HLAfloat64TimeFactory)this.rtiamb.getTimeFactory();

        this.rtiamb.registerFederationSynchronizationPoint(PrzygotowujacyFederate.READY_TO_RUN, null);
        while(this.fedamb.isAnnounced == false)
        {
                this.rtiamb.evokeMultipleCallbacks(0.1, 0.2);
        }
        this.waitForUser();

        this.rtiamb.synchronizationPointAchieved(PrzygotowujacyFederate.READY_TO_RUN);
        this.log("Achieved sync point: " + PrzygotowujacyFederate.READY_TO_RUN + ", waiting for federation...");
        while(this.fedamb.isReadyToRun == false)
        {
                this.rtiamb.evokeMultipleCallbacks(0.1, 0.2);
        }

        this.enableTimePolicy();
        this.log("Time Policy Enabled.");

        this.publishAndSubscribe();
        this.log("Published and Subscribed.");

        ObjectInstanceHandle objectHandle = this.registerObject();
        this.log("Registered Object, handle=" + objectHandle);

        for(int i = 0; i < 30; i++)
        {
                this.updateAttributeValues(objectHandle);
                this.advanceTime(1.0);
        }

        if (state == 0)
            this.log("Process with id: " + this.id + " on state IDLE was starved");

        else if (state == 1)
        {
            while(state == 1)
            {
                this.updateAttributeValues(objectHandle);
                this.advanceTime(1.0);
            }
        }

        this.log("Process with id: " + this.id + " was finished. Removing this process ...");
        this.deleteObject(objectHandle);
        this.log("Deleted Object, handle=" + objectHandle);
        this.rtiamb.resignFederationExecution(ResignAction.DELETE_OBJECTS);
        this.log("Resigned from Federation.");
        try
        {
                this.rtiamb.destroyFederationExecution("MskFederation");
                this.log("Destroyed Federation.");
        }
        catch(FederationExecutionDoesNotExist dne)
        {
                this.log("No need to destroy federation, it doesn't exist.");
        }
        catch(FederatesCurrentlyJoined fcj)
        {
                this.log("Didn't destroy federation, federates still joined.");
        }
    }

    private void enableTimePolicy() throws Exception
    {
        HLAfloat64Interval lookahead = this.timeFactory.makeInterval(this.fedamb.federateLookahead);
        this.rtiamb.enableTimeRegulation(lookahead);

        while(this.fedamb.isRegulating == false)
            this.rtiamb.evokeMultipleCallbacks(0.1, 0.2);

        this.rtiamb.enableTimeConstrained();

        while(fedamb.isConstrained == false)
            rtiamb.evokeMultipleCallbacks(0.1, 0.2);
    }
}
