/*
 * Raud Kelly
 * Assignment 02
 * Inspection Event Class
 * 
 */
import desmoj.core.simulator.*;
public class InspectionEvent extends Event<Part>{

	public InspectionEvent(Model owner, String name, boolean showInTrace) {
		super(owner, name, showInTrace);
	}

	public void eventRoutine(Part part) {
		ShopModel model = (ShopModel) this.getModel();
		model.inspectorIdle = false;
		
		boolean defect = model.needsRefine.sample();
		
		if(defect) {
			//schedules refining here
			if(model.machineIdle) {
				model.machineIdle = false;
				double refineTime = model.refining.sample();
				ProcessingEvent refine = new ProcessingEvent(model, "Part", true);
				refine.schedule(part, new TimeSpan(refineTime));
			} else {
				model.partQueue.insert(part);
			}
		} else {
			
			//updating statistics
			//response time
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
		}
		
		if(!model.inspectQueue.isEmpty()) {
			Part nextPart = model.inspectQueue.first();
			model.inspectQueue.remove(nextPart);
			double inspectionTime = model.inspection.sample();
			InspectionEvent inspect = new InspectionEvent(model, "Part", true);
			inspect.schedule(nextPart, new TimeSpan(inspectionTime));
		} else {
			model.inspectorIdle = true;
		}
		
	}

}
