/*
 * Raud Kelly
 * Assignment 04
 * Part Process Class
 * 
 */
import java.util.concurrent.TimeUnit;

import co.paralleluniverse.fibers.SuspendExecution;
import desmoj.core.simulator.*;
public class Part extends SimProcess {

	protected double arrivalTime;
	protected double leaveTime;
	
	public Part(Model owner, String name, boolean showInTrace) {
		super(owner, name, showInTrace);
		
		//to calculate response statistics
		arrivalTime = 0;
		leaveTime = 0;
	}
	
	//process life cycle
	public void lifeCycle() throws SuspendExecution {
		ShopModel model = (ShopModel) this.getModel();
		this.arrivalTime = model.presentTime().getTimeAsDouble();
		model.totalParts++;
		
		if(model.totalParts > model.maxParts) {
			model.maxParts = model.totalParts;
		}
		/*
		 * Part Arrival section
		 */
		Part part = new Part(model, "Part", true);
		double interArrivalTime = model.interArrivalTimes.sample();
		part.activate(new TimeSpan(interArrivalTime, TimeUnit.MINUTES));
		
		
		/*
		 * Processing section
		 */
		model.partQueue.insert(this);
		
		//check if the machine is idle
		if(!model.machineIdle) {
			this.passivate();
		} else {
			model.partQueue.remove(this);
		}
		
		//commence processing
		model.machineIdle = false;
		//update utilization rate start time
		model.lastMachine = model.presentTime().getTimeAsDouble();
		double processTime = model.processing.sample();
		this.hold(new TimeSpan(processTime));
		
		//update utilization rate end time and add to total
		model.totalMachine = model.totalMachine + (model.presentTime().getTimeAsDouble() - model.lastMachine);
		
		//check if there are parts waiting to be processed
		if(!model.partQueue.isEmpty()) {
			model.partQueue.removeFirst().activate();
		} else {
			model.machineIdle = true;
			
		}
		
		/*
		 * Inspecting section
		 */
		model.inspectQueue.insert(this);
		
		if(!model.inspectorIdle) {
			this.passivate();
		} else {
			model.inspectQueue.remove(this);
		}
		
		//commence inspection
		model.inspectorIdle = false;
		//update utilization rate start time
		model.lastInspect = model.presentTime().getTimeAsDouble();
		double inspectTime = model.inspection.sample();
		this.hold(new TimeSpan(inspectTime));
		
		//check for defects
		boolean defect = model.needsRefine.sample();

		//update utilization rate end time and add to total
		model.totalInspect = model.totalInspect + (model.presentTime().getTimeAsDouble() - model.lastInspect);
		
		//check if there are parts waiting to be processed
		if(!model.inspectQueue.isEmpty()) {
			model.inspectQueue.removeFirst().activate();
		} else {
			model.inspectorIdle = true;
			
		}
		
		/*
		 * Refining section
		 */
		if(defect) {
			
			model.partQueue.insert(this);
			
			//check if the machine is idle
			if(!model.machineIdle) {
				this.passivate();
			} else {
				model.partQueue.remove(this);
			}
			
			//commence processing
			model.machineIdle = false;
			//update utilization rate start time
			model.lastMachine = model.presentTime().getTimeAsDouble();
			double refineTime = model.refining.sample();
			this.hold(new TimeSpan(refineTime));
			
			//update utilization rate end time and add to total
			model.totalMachine = model.totalMachine + (model.presentTime().getTimeAsDouble() - model.lastMachine);
			
			//check if there are parts waiting to be processed
			if(!model.partQueue.isEmpty()) {
				model.partQueue.removeFirst().activate();
			} else {
				model.machineIdle = true;
			}
		}
		
		//updating end statistics
		//response time
		this.leaveTime = model.presentTime().getTimeAsDouble();
		double response = (this.leaveTime - this.arrivalTime);
		
		//max and min response time
		if(response > model.maxResponse) {
			model.maxResponse = response;
		} if (response < model.minResponse) {
			model.minResponse = response;
		}
		
		//updating total response time to calculate avg at the end
		model.totalResponse = model.totalResponse + response;
			
		//updating total parts in the system and # of parts processed
		model.totalParts--;
		model.totalPartsProcessed++;
	}

}
