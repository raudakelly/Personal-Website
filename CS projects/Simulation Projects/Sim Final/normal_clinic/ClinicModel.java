/*
 * Raud Kelly
 * Simulation Project
 * Clinic Model Class
 * 
 */

import desmoj.core.simulator.*;

import java.util.concurrent.TimeUnit;

import desmoj.core.dist.*;
public class ClinicModel extends Model {
	
	//structures
	protected desmoj.core.simulator.ProcessQueue<Patient> waitQueue;
	protected desmoj.core.simulator.ProcessQueue<Patient> examQueue;
	
	//distributions
	protected ContDistExponential morningArrival;
	protected ContDistExponential noonArrival;
	protected ContDistExponential nightArrival;
	protected ContDistExponential treatment;
	protected ContDistExponential specTreatment;
	protected BoolDistBernoulli refer;
	
	//statistical trackers
	protected int expenses;
	protected int numBalk;
	protected int totalPatients;
	protected int diverted;
	protected int fullyTreated;
	protected double response;
	
	//utilization rate trackers
	protected double lastTimeNurse;
	protected double lastTimeSpec;
	protected double totalNurse;
	protected double totalSpec;
	
	//wait room #
	protected int waiting;
	
	//state variables
	protected boolean nurseIdle;
	protected boolean specIdle;
	
	public ClinicModel(Model owner, String modelName, boolean showInReport, boolean showInTrace) {
		super(owner, modelName, showInReport, showInTrace);
		
	}

	public String description() {
		return "I'm a clinic!";
	}

	public void doInitialSchedules() {
		
		//creating first patient
		Patient part = new Patient(this, "Part", true);
		double interArrivalTime = this.morningArrival.sample();
		part.activate(new TimeSpan(interArrivalTime, TimeUnit.MINUTES));
		
	}

	public void init() {		
		waitQueue = new ProcessQueue<Patient>(this, "Wait Room Queue", true, true);
		examQueue = new ProcessQueue<Patient>(this, "Exam Room Queue", true, true);
		
		
		//initializing statistical variables
		//we have 1 nurse, 1 specialist, 4 exam rooms in this configuration so
		expenses = 3900;
		numBalk = 0;
		totalPatients = 0;
		diverted = 0;
		fullyTreated = 0;
		response = 0;
		
		//utilization rate trackers
		lastTimeNurse = 0;
		lastTimeSpec = 0;
		
		//waiting room numbers
		waiting = 0;
		
		//state variables
		nurseIdle = true;
		specIdle = true;
		
		//initializing distributions
		morningArrival = new ContDistExponential(this, "ArrivalTimes", 15.0, true, true);
		noonArrival = new ContDistExponential(this, "Processing", 6.0, true, true);
		nightArrival = new ContDistExponential(this, "Inspection", 9.0, true, true);
		treatment = new ContDistExponential(this, "Refining", 8.0, true, true);
		specTreatment = new ContDistExponential(this, "Refining", 25.0, true, true);
		refer = new BoolDistBernoulli(this, "RefineProbability", .4, true, true);
	}
	
	//running the simulation
	public static void main(String args[]) {
		//set the reference in units to minutes
		Experiment.setReferenceUnit(TimeUnit.MINUTES);
		//create model and run experiment
		ClinicModel model = new ClinicModel(null, "Clinic Model", true, true);
		
		Experiment exp = new Experiment("Clinic Model Experiment");
		model.connectToExperiment(exp);
		
		//run clinic for a day, but nothing happens after the specialist and nurse leaves
		exp.stop(new TimeInstant(24, TimeUnit.HOURS));
		exp.start();
		exp.report();
		//report statistics
		System.out.println("Total expenses:             $" + model.expenses);
		System.out.println("Total Patients arrived:      " + model.totalPatients);
		System.out.println("# of balked:                 " + model.numBalk);
		System.out.println("# of diverted to ER:         " + model.diverted);
		System.out.println("# of fully treated:          " + model.fullyTreated);
		System.out.println("Average response time:       " + model.response / model.fullyTreated);
		System.out.println("Util Rate for Nurse(%):      " + (model.totalNurse/ model.lastTimeNurse) * 100);
		System.out.println("Util Rate for Specialist(%): " + (model.totalSpec/ model.lastTimeSpec) * 100);
		exp.finish();
	}

}
