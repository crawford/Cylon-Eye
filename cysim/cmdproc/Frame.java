class Frame {
	private int time;
	private String data;
	private int gotoAddress;
	
	public Frame(byte inst[]) {
		time = ((inst[0] & 0xFF) << 8) + (inst[1] & 0xFF);
		
		if(time == 0) {
			//Encoded data is an address
			gotoAddress = ((inst[2] & 0xFF) << 8) + (inst[3] & 0xFF);
		} else {
			//Encoded data is display data
		
			//Convert last 6 bytes of inst to array of boolean
			int bits[] = new int[48];
			for(int b = 2; b < 8; b++) {
				for(int p = 0; p < 8; p++) {
					bits[(b - 2)*8 + p] = (inst[b] << p) & 0x80;
				}
			}
			
			//Convert 3-bit values to integers and store in data
			data = "";
			for(int i = 0; i < 16; i++) {
				data += bits[3*i + 2]*4 + bits[3*i + 1]*2 + bits[3*i];
			}
		}
	}
	
	public boolean isGoto() {
		return (time == 0);
	}
	
	public int getGotoAddress() {
		return gotoAddress;
	}
	
	public String getDisplayData() {
		return data;
	}
}