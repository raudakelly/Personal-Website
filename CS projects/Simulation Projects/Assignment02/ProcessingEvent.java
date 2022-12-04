/*
 * Raud Kelly
 * Assignment 02
 * Processing Event Class
 * 
 */
import desmoj.core.simulator.*;
public class ProcessingEvent extends Event<Part>{

	public ProcessingEvent(Model owner, String name, boolean showInTrace) {
		super(owner, name, showInTrace);
	}

	public void eventRoutine(Part part) {
		ShopModel model = (ShopModel) this.getModel();
		model.machineIdle = false;
		
		if(part.hasProcessed) {
			//part is done in the system, collect end stats
			part.leaveTime = model.presentTime().getTimeAsDouble();
			double response = (part.leaveTime - part.arrivalTime);
			
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
		} else {
			part.hasProcessed = true;
			if(model.inspectorIdle) {
				model.inspectorIdle = false;
				double inspectionTime = model.inspection.sample();
				InspectionEvent inspect = new InspectionEvent(model, "Part", true);
				inspect.schedule(part, new TimeSpan(inspectionTime));
			} else {
				model.inspectQueue.insert(part);
			}
		}
		
		//if there are no parts in the queue, waits until a part arrives
		//then continuously calls itself until the partsQueue is empty again
		//this ensures that when parts are waiting the machine is constantly processing
		if(!model.partQueue.isEmpty()) {
			Part nextPart = model.partQueue.first();
			model.partQueue.remove(nextPart);
			if(nextPart.hasProcessed) {
				double refineTime = model.refining.sample();
				ProcessingEvent refine = new ProcessingEvent(model, "Part", true);
				refine.schedule(nextPart, new TimeSpan(refineTime));
			} else {
				double processTime = model.processing.sample();
				ProcessingEvent process = new ProcessingEvent(model, "Part", true);
				process.schedule(nextPart, new TimeSpan(processTime));
			}
			
		} else {
			model.machineIdle = true;
		}
	}

}
