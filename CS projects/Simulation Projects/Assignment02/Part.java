/*
 * Raud Kelly
 * Assignment 02
 * Part Entity Class
 * 
 */
import desmoj.core.simulator.*;
public class Part extends Entity {

	protected double arrivalTime;
	protected double leaveTime;
	protected boolean hasProcessed;
	
	public Part(Model owner, String name, boolean showInTrace) {
		super(owner, name, showInTrace);
		
		//to calculate response statistics
		arrivalTime = 0;
		leaveTime = 0;
		
		//to decide whether a part needs to be processed or
		//refined when it reaches the machine
		hasProcessed = false;
	}
}
