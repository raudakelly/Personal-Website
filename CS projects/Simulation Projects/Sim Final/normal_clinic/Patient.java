/*
 * Raud Kelly
 * Simulation Project
 * Patient Process Class
 */

import java.util.Random;
import java.util.concurrent.TimeUnit;

import co.paralleluniverse.fibers.SuspendExecution;
import desmoj.core.simulator.*;
public class Patient extends SimProcess {

	protected double arriveTime;
	
	public Patient(Model owner, String name, boolean showInTrace) {
		super(owner, name, showInTrace);
	}

	//Note: for simplicity sake, 8am = time 0 in execution
	//7:59am = last moment in time of simulation
	public void lifeCycle() throws SuspendExecution {
		ClinicModel model = (ClinicModel) this.getModel();
		model.totalPatients++;
		
		//if clinic is open, generate and schedule the next patient
		if(model.presentTime().getTimeAsDouble() < 720) {
			Patient p = new Patient(model, "Patient", true);
			double arrTime;
			//generate arrival based on time of day
			if(model.presentTime().getTimeAsDouble() < 120) {
				arrTime = model.morningArrival.sample();
			} else if(model.presentTime().getTimeAsDouble() < 480) {
				arrTime = model.noonArrival.sample();
			} else {
				arrTime = model.nightArrival.sample();
			}
			p.activate(new TimeSpan(arrTime, TimeUnit.MINUTES));
		}
		
		Random r = new Random();
		int balks = r.nextInt(8) + 1;
		
		//checks if patient balks, if 8 people are waiting
		//balks is always <= waiting, therefore they balk
		//if waiting = 0, condition is automatically false
		//and the patient does not balk
		if(balks <= model.waiting) {
			//statistics and return
			model.expenses = model.expenses + 500;
			model.numBalk++;
			return;
		}
		
		arriveTime = model.presentTime().getTimeAsDouble();
		
		/*
		 * Nurse Treatment Section
		 */
		model.waiting++;
		model.waitQueue.insert(this);
		
		if(!model.nurseIdle) {
			this.passivate();
		} else {
			model.waitQueue.remove(this);
		}
		model.waiting--;
		model.nurseIdle = false;
		double treatTime = model.treatment.sample();
		model.totalNurse = model.totalNurse + treatTime;
		boolean refer = model.refer.sample();
		this.hold(new TimeSpan(treatTime));
		
		if(!model.waitQueue.isEmpty()) {
			model.waitQueue.removeFirst().activate();
		} else {
			model.lastTimeNurse = model.presentTime().getTimeAsDouble();
			model.nurseIdle = true;
		}
		
		if(!refer) {
			//end statistics and return
			model.expenses = model.expenses + 100;
			model.response = model.response + (model.presentTime().getTimeAsDouble() - arriveTime);
			model.fullyTreated++;
			return;
		}
		
		//if patient has been in for more than 30 mins send to emergency care
		if((model.presentTime().getTimeAsDouble() - arriveTime) > 30) {
			//end statistics and return
			model.expenses = model.expenses + 600;
			model.diverted++;
			return;
		}
		
		if(model.examQueue.size() >= 3) {
			//send to emergency care
			model.expenses = model.expenses + 600;
			model.diverted++;
			return;
		}
		
		/*
		 * Specialist treatment section
		 */
		
		model.examQueue.insert(this);
		if(!model.specIdle) {
			this.passivate();
		} else {
			model.examQueue.remove(this);
		}
		
		model.specIdle = false;
		//commence specialist treatment	
		double specTreatTime = model.specTreatment.sample();
		model.totalSpec = model.totalSpec + specTreatTime;
		this.hold(new TimeSpan(specTreatTime));
		
		//if there are more people in exam rooms, continue to treat them
		if(!model.examQueue.isEmpty()) {
			model.examQueue.removeFirst().activate();
		} else {
			model.lastTimeSpec = model.presentTime().getTimeAsDouble();
			model.specIdle = true;
		}
		model.expenses = model.expenses + 200;
		model.response = model.response + (model.presentTime().getTimeAsDouble() - arriveTime);
		model.fullyTreated++;
	}

}