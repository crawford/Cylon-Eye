import java.util.ArrayList;

class Zigbee {
	private String ZID;
	private ArrayList<Panel> panels;
	int curChannel;
	
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