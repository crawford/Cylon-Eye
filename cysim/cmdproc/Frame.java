class Frame {
	private int time;
	private String displayData;
	private int gotoAddress;
	
	//inst - 6 byte string in the format TTDDDD, where TT is a time value
	// and DDDD and is display data, or in the format 00AA00, where AA is
	// an address
	public Frame(byte inst[]) {
		time = ((inst[0] & 0xFF) << 8) + (inst[1] & 0xFF);
		
		//Seperate the 4 bytes of data
		byte data[] = new byte[6];
		for(int i = 0; i < 6; i++) {
			data[i] = inst[i + 2];
		}
		
		if(time == 0) {
			//Encoded data is an address
			gotoAddress = toGotoAddress(data);
		} else {
			//Encoded data is display data
			displayData = toDisplayData(data);
		}
	}
	
	public boolean isGoto() {
		return (time == 0);
	}
	
	public int getGotoAddress() {
		return gotoAddress;
	}
	
	public String getDisplayData() {
		return displayData;
	}
	
	public static String toDisplayData(byte data[]) {
		String displayString = "";
		
		//Convert the 6 bytes of data to array of booleans
		int bits[] = new int[48];
		for(int b = 0; b < 6; b++) {
			for(int p = 0; p < 8; p++) {
				bits[b*8 + p] = (data[b] << p) & 0x80;
			}
		}
		
		//Convert 3-bit values to integers and store in displayString
		for(int i = 0; i < 16; i++) {
			displayString += bits[3*i + 2]*4 + bits[3*i + 1]*2 + bits[3*i];
		}
		
		return displayString;
	}
	
	public static int toGotoAddress(byte data[]) {
		return ((data[0] & 0xFF) << 8) + (data[1] & 0xFF);
	}
}