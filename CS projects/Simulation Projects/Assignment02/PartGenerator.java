import desmoj.core.simulator.*;
public class PartGenerator extends ExternalEvent{

	public PartGenerator(Model owner, String name, boolean showInTrace) {
		super(owner, name, showInTrace);
	}

	public void eventRoutine() {
		ShopModel model = (ShopModel) this.getModel();
		
		//making next part
		Part part = new Part(model, "Part", true);
		double interArrivalTime = model.interArrivalTimes.sample();
		ArrivalEvent arrival = new ArrivalEvent(model, "Part", true);
		arrival.schedule(part, new TimeSpan(interArrivalTime));
		
		PartGenerator nextGen = new PartGenerator(model, "Generator", true);
        nextGen.schedule(new TimeSpan(interArrivalTime));
	}

}
