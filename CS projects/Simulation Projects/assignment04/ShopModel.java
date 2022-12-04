/*
 * Raud Kelly
 * Assignment 04
 * Model Class
 * Answers:
 * Minimum Response: 			3.73
 * Maximum Response: 		  163.43
 * Average Response: 		   45.03
 * Maximum Parts:				17
 * Average Processing Queue:	4.23
 * Maximum Processing Queue:    15
 * Average Inspection Queue:	0.867
 * Maximum Inspection Queue:	6
 * Machine Utilization Rate:   86.95%
 * Inspector Utilization Rate: 73.63%
 * 
 */

import desmoj.core.simulator.*;

import java.util.concurrent.TimeUnit;

import desmoj.core.dist.*;
public class ShopModel extends Model {
	
	//structures
	protected desmoj.core.simulator.ProcessQueue<Part> partQueue;
	protected desmoj.core.simulator.ProcessQueue<Part> inspectQueue;
	
	//distributions
	protected ContDistExponential interArrivalTimes;
	protected ContDistExponential processing;
	protected ContDistNormal inspection;
	protected ContDistExponential refining;
	protected BoolDistBernoulli needsRefine;
	
	
	/*For testing the code to make sure it works
	protected ContDistUniform interArrivalTimes;
	protected ContDistUniform processing;
	protected ContDistUniform inspection;
	protected ContDistUniform refining;
	protected BoolDistBernoulli needsRefine;
	*/
	
	//statistical trackers
	protected int totalParts;
	protected int totalPartsProcessed;
	protected double minResponse;
	protected double maxResponse;
	protected double totalResponse;
	protected int maxParts;
	
	//state variables
	protected boolean machineIdle;
	protected boolean inspectorIdle;
	
	//machine utilization variables
	protected double lastMachine;
	protected double totalMachine;
	protected double lastInspect;
	protected double totalInspect;
	
	public ShopModel(Model owner, String modelName, boolean showInReport, boolean showInTrace) {
		super(owner, modelName, showInReport, showInTrace);
		
	}

	public String description() {
		
		return null;
	}

	public void doInitialSchedules() {
		
		//creating first part, sending it out, and updating total time
		Part part = new Part(this, "Part", true);
		double interArrivalTime = this.interArrivalTimes.sample();
		part.activate(new TimeSpan(interArrivalTime, TimeUnit.MINUTES));
		
	}

	public void init() {		
		partQueue = new ProcessQueue<Part>(this, "Process Queue", true, true);
		inspectQueue = new ProcessQueue<Part>(this, "Inspection Queue", true, true);
		
		
		//initializing statistical variables
		totalParts = 0;
		totalPartsProcessed = 0;
		minResponse = 100.0;
		maxResponse = 0;
		totalResponse = 0;
		maxParts = 0;
		
		//state variables
		machineIdle = true;
		inspectorIdle = true;
		
		//utilization variables
		lastMachine = 0;
		totalMachine = 0;
		lastInspect = 0;
		totalInspect = 0;
		
		//initializing distributions
		interArrivalTimes = new ContDistExponential(this, "ArrivalTimes", 7.0, true, true);
		processing = new ContDistExponential(this, "Processing", 4.5, true, true);
		inspection = new ContDistNormal(this, "Inspection", 5.0, 1.0, true, true);
		refining = new ContDistExponential(this, "Refining", 3.0, true, true);
		needsRefine = new BoolDistBernoulli(this, "RefineProbability", .25, true, true);
		
		
		/* For testing the code to make sure it works
		interArrivalTimes = new ContDistUniform(this, "ArrivalTimes", 2.0, 12.0, true, true);
		processing = new ContDistUniform(this, "Processing", 3.0, 9.0, true, true);
		inspection = new ContDistUniform(this, "Inspection", 1.0, 9.0, true, true);
		refining = new ContDistUniform(this, "Refining", 2.0, 4.0, true, true);
		needsRefine = new BoolDistBernoulli(this, "RefineProbability", .25, true, true);
		*/
	}
	
	//running the simulation
	public static void main(String args[]) {
		//set the reference in units to minutes
		Experiment.setReferenceUnit(TimeUnit.MINUTES);
		//create model and run experiment
		ShopModel model = new ShopModel(null, "Shop Model", true, true);
		Experiment exp = new Experiment("Shop Model Experiment (04)");
		model.connectToExperiment(exp);
		exp.stop(new TimeInstant(24, TimeUnit.HOURS));
		exp.start();
		exp.report();
		//report statistics
		System.out.println("Avg Response: " + (double)(model.totalResponse/model.totalPartsProcessed));
		System.out.println("Max Response: " + model.maxResponse);
		System.out.println("Min Response: " + model.minResponse);
		System.out.println("Max Parts: " + model.maxParts);
		System.out.println("Processor Utilization Rate(%): " + (model.totalMachine / 1440) * 100);
		System.out.println("Inspector Utilization Rate(%): " + (model.totalInspect / 1440) * 100);
		exp.finish();
	}

}
