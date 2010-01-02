import java.util.ArrayList;

public class Zigbee {
	private String ZID;
	private ArrayList<Panel> panels;
	private int curChannel;
	
	public Zigbee(String zid) {
		ZID = zid;
		panels = new ArrayList<Panel>();
	}
	
	public String getZID() {
		return ZID;
	}
	
	public void addPanel(Panel panel) {
		panels.add(panel);
	}
	
	public void evaluate(byte command[]) {
	
	}
}