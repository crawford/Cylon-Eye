import java.util.ArrayList;

public class Zigbee {
	private String ZID;
	private Panel panels[];
	private byte curChannel;

	static final String HEXES = "0123456789ABCDEF";
	public static String getHex( byte [] raw ) {
		if ( raw == null ) {
			return null;
		}
		final StringBuilder hex = new StringBuilder( 2 * raw.length );
		for ( final byte b : raw ) {
			hex.append(HEXES.charAt((b & 0xF0) >> 4))
				.append(HEXES.charAt((b & 0x0F)));
		}
		return hex.toString();
	}


	public Zigbee(String zid) {
		ZID = zid;
		panels = new Panel[8];
	}

	public String getZID() {
		return ZID;
	}

	public void addPanel(Panel panel) {
		if(panel.getPID() > 8)
			return;

		panels[panel.getPID()] = panel;
	}

	public void evaluate(byte command[]) {
		//Make sure start delimeter is present
		if(command[0] != 0xFE)
			return;

		//Pull the address from the command
		byte address[] = new byte[8];
		for(int i = 0; i < 8; i++) {
			address[i] = command[i + 6];
		}

		//Make sure the message is addressed to this zigbee or broadcast
		if(!ZID.equals("000000000000FFFF") && !ZID.equals(getHex(address)))
			return;

		if(command[3] == 0x00) {
			//If the message is an RX request (64 bit address)

			//Pull the data from the command
			byte message[] = new byte[command.length - 16];
			for(int i = 0; i < message.length; i++) {
				message[i] = command[15 + i];
			}

			//Determine which panels to send the message to
			for(int i = 0; i < 8; i++) {
				if(((curChannel >> i) & 0x01) == 1) {
					if(panels[i] != null)
						panels[i].evaluate(message);
				}
			}

		} else if(command[3] == 0x08) {
			//If the message is an AT command
			
			if(command[5] == 'I' && command[6] == 'O') {
				//If the AT command is an I/O write
				
				curChannel = command[7];
			}
		}
	}
}
