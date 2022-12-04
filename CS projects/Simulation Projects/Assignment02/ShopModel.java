/*
 * Raud Kelly
 * Assignment 02
 * Model Class
 * Answers:
 * Minimum Response: 			6.31
 * Maximum Response: 		   70.48
 * Average Response: 		   25.16
 * Maximum Parts:				9
 * Average Processing Queue:	1.71
 * Maximum Processing Queue:	7
 * Average Inspection Queue:	0.24
 * Maximum Inspection Queue:	3
 * 
 * Note: I took the queue statistics from the
 * generated report.
 */

import desmoj.core.simulator.*;
import java.util.concurrent.TimeUnit;

import desmoj.core.dist.*;
public class ShopModel extends Model {
	//structures
	protected desmoj.core.simulator.Queue<Part> partQueue;
	protected desmoj.core.simulator.Queue<Part> inspectQueue;
	
	//state variables
	protected boolean machineIdle;
	protected boolean inspectorIdle;
	
	//distributions
	protected ContDistUniform interArrivalTimes;
	protected ContDistUniform processing;
	protected ContDistUniform inspection;
	protected ContDistUniform refining;
	protected BoolDistBernoulli needsRefine;
	
	//statistical trackers
	protected double totalTime;
	protected int totalParts;
	protected int totalPartsProcessed;
	protected double minResponse;
	protected double maxResponse;
	protected double totalResponse;
	protected int maxParts;
	

	public ShopModel(Model owner, String modelName, boolean showInReport, boolean showInTrace) {
		super(owner, modelName, showInReport, showInTrace);
		
	}

	public String description() {
		
		return null;
	}

	public void doInitialSchedules() {
		PartGenerator start = new PartGenerator(this, "Part", true);
		start.schedule();
	}

	public void init() {		
		partQueue = new Queue<Part>(this, "Process Queue", true, true);
		inspectQueue = new Queue<Part>(this, "Inspection Queue", true, true);
		
		machineIdle = true;
		inspectorIdle = true;
		
		//initializing statistical variables
		totalParts = 0;
		totalTime = 0;
		totalPartsProcessed = 0;
		minResponse = 100.0;
		maxResponse = 0;
		totalResponse = 0;
		maxParts = 0;

		//initializing distributions
		interArrivalTimes = new ContDistUniform(this, "ArrivalTimes", 2.0, 12.0, true, true);
		processing = new ContDistUniform(this, "Processing", 3.0, 9.0, true, true);
		inspection = new ContDistUniform(this, "Inspection", 1.0, 9.0, true, true);
		refining = new ContDistUniform(this, "Refining", 2.0, 4.0, true, true);
		needsRefine = new BoolDistBernoulli(this, "RefineProbability", .25, true, true);
	}
	
	//running the simulation
	public static void main(String args[]) {
		//set the reference in units to minutes
		Experiment.setReferenceUnit(TimeUnit.MINUTES);
		//create model and run experiment
		ShopModel model = new ShopModel(null, "Shop Model", true, true);
		Experiment exp = new Experiment("Shop Model Experiment");
		model.connectToExperiment(exp);
		exp.stop(new TimeInstant(24, TimeUnit.HOURS));
		exp.start();
		exp.report();
		//report statistics
		System.out.println("Avg Response: " + (double)(model.totalResponse/model.totalPartsProcessed));
		System.out.println("Max Response: " + model.maxResponse);
		System.out.println("Min Response: " + model.minResponse);
		System.out.println("Max Parts: " + model.maxParts);
		exp.finish();
	}

}
