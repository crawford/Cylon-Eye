import java.util.ArrayList;

class Panel {
	private String ZID;
	private String PID;
	private ArrayList<Frame> frames;
	private String data;
	private boolean auxLED;
	//private ??? timer;
	
	public Panel(String zid, String pid) {
		ZID = zid;
		PID = pid;
		frames = new ArrayList<Frame>();
	}
}