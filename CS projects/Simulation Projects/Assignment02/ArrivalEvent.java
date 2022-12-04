/*
 * Raud Kelly
 * Assignment 02
 * InterArrival Event Class
 * 
 */
import desmoj.core.simulator.*;
public class ArrivalEvent extends Event<Part> {

	public ArrivalEvent(Model owner, String name, boolean showInTrace) {
		super(owner, name, showInTrace);
	}

	public void eventRoutine(Part part) {
		ShopModel model = (ShopModel) this.getModel();
		
		//update part arrival time
		part.arrivalTime = model.presentTime().getTimeAsDouble();
		
		//if the machine is idle, schedule
		//else add to the queue
		if(model.machineIdle) {
			model.machineIdle = false;
			double processTime = model.processing.sample();
			ProcessingEvent process = new ProcessingEvent(model, "Part", true);
			process.schedule(part, new TimeSpan(processTime));
		} else {
			model.partQueue.insert(part);
		}
			
		//update total parts in system
		model.totalParts++;
		
		//update max parts statistic
		if(model.totalParts > model.maxParts) {
			model.maxParts = model.totalParts;
		}

		
	}

}
