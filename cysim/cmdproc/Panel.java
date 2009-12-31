import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;

class Panel {
	private final double TIMER_SCALE_FACTOR = 1;	// milliseconds / Panel-Time-Period
	private String ZID;
	private String PID;
	private ArrayList<Frame> frames;
	private String displayData;
	private boolean auxLED;
	private int pointer;
	private int period;
	private Timer timer;
	private int currentFrameTime;
	
	public Panel(String zid, String pid) {
		ZID = zid;
		PID = pid;
		frames = new ArrayList<Frame>();
		setDisplay("0000000000000000");
		auxLED = false;
		pointer = 0;
		timer = new Timer();
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
				pause();
				setDisplay(Frame.toDisplayData(payload));
				break;
			case 'L':
				//Turn on/off the auxilary LED
				setLED(payload[0] != 0);
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
				pause();
				resume();
				break;
			case 'R':
				//Reset and pause the animation
				pause();
				pointer = 0;
				break;
			case 'P':
				//Pause the animation
				pause();
				break;
			case 'T':
				//Set animation speed
				pause();
				period = Frame.toTimeValue(payload);
				resume();
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
	
	public String toString() {
		return ZID + "-" + PID + "-" + displayData + "-" + (auxLED ? "1" : "0");
	}
	
	private void pause() {
		timer.cancel();
		timer = new Timer();
	}
	
	private void resume() {
		if(frames.size() > 0)
			timer.scheduleAtFixedRate(new PanelTask(), (long)0, (long)(period*TIMER_SCALE_FACTOR));
	}
	
	private void setDisplay(String data) {
		displayData = data;
		System.out.println(this);
	}
	
	private void setLED(boolean led) {
		auxLED = led;
		System.out.println(this);
	}
	
	private class PanelTask extends TimerTask {
		public void run() {
			if(currentFrameTime > 0) {
				currentFrameTime--;
				return;
			}
			
			if(pointer >= frames.size()) {
				pointer = 0;
			}

			Frame frame = frames.get(pointer);
			if(frame.isGoto()) {
				pointer = frame.getGotoAddress();
				frame = frames.get(pointer);
			} 
			
			setDisplay(frame.getDisplayData());
			currentFrameTime = frame.getDisplayTime();
			pointer++;
		}
	}
}