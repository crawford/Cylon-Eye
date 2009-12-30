import java.util.ArrayList;

class Panel {
	private String ZID;
	private String PID;
	private ArrayList<Frame> frames;
	private String data;
	private boolean auxLED;
	private int pointer;
	//private ??? timer;
	
	public Panel(String zid, String pid) {
		ZID = zid;
		PID = pid;
		frames = new ArrayList<Frame>();
		data = "0000000000000000";
		auxLED = false;
		pointer = 0;
	}
	
	public void evaluate(byte command[]) {
		//Strip the command character from the front and the newline
		// from the back
		byte payload[] = new byte[command.length - 2];
		for(int i = 0; i < command.length - 2; i++) {
			payload[i] = command[i + 1];
		}
			
		switch(command[0]) {
			case 'C':
				//Overwrite the current display data and stop animation
				data = Frame.toDisplayData(payload);
				//TODO
				break;
			case 'L':
				//Turn on/off the auxilary LED
				if(payload[0] == 0) {
					auxLED = true;
				} else {
					auxLED = false;
				}
				break;
			case 'D':
				//Write frame to current pointer and increment
				frames.add(new Frame(payload));
				pointer++;
				break;
			case 'X':
				//Set pointer to specified value
				pointer = Frame.toGotoAddress(payload);
				break;
			case 'S':
				break;
			case 'P':
				break;
			case 'R':
				break;
			case 'T':
				break;
			case 'N':
				break;
			case 'B':
				break;
			case 'Y':
				break;
		}
	}
}