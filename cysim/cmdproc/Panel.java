import java.util.ArrayList;

class Panel {
	private String ZID;
	private String PID;
	private ArrayList<Frame> frames;
	private String displayData;
	private boolean auxLED;
	private int pointer;
	//private ??? timer;
	
	public Panel(String zid, String pid) {
		ZID = zid;
		PID = pid;
		frames = new ArrayList<Frame>();
		displayData = "0000000000000000";
		auxLED = false;
		pointer = 0;
	}
	
	public String evaluate(byte command[]) {
		//Strip the command character from the front and the newline
		// from the back
		byte payload[] = new byte[command.length - 2];
		for(int i = 0; i < command.length - 2; i++) {
			payload[i] = command[i + 1];
		}
			
		switch(command[0]) {
			case 'C':
				//Overwrite the current display data and stop animation
				displayData = Frame.toDisplayData(payload);
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
				//Start the animation from the current pointer value
				break;
			case 'R':
				//Reset and pause the animation
				pointer = 0;
			case 'P':
				//Pause the animation
				break;
			case 'T':
				//Set animation speed
				break;
			case 'N':
				//No operation
				break;
			case 'B':
				return "B" + Frame.toRawDisplayData(displayData) + "\n";
			case 'Y':
				return "Y" + Frame.toRawGotoAddress(pointer) + "\n";
		}
		
		return "";
	}
}