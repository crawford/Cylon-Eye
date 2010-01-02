public class CmdProc {
	public static void main(String args[]) {
		
	
		byte command[];
		Panel test = new Panel("0013A2004031A660", "1");
		
		//Set display
		command = new byte[]{'C', 0x05, 0x39, 0x77, (byte)0xFA, (byte)0xC6, (byte)0x88, '\n'};
		test.evaluate(command);
		
		//Turn aux led on
		command = new byte[]{'L', 0x05, '\n'};
		test.evaluate(command);
		
		//Turn aux led off
		command = new byte[]{'L', 0x00, '\n'};
		test.evaluate(command);
		
		//Turn aux led on
		command = new byte[]{'L', 0x17, '\n'};
		test.evaluate(command);

		//Set standard cylon eye animation
		command = new byte[]{'X', 0x00, 0x00, '\n'};
		test.evaluate(command);
		
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0xE0, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0xFC, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0xFF, (byte)0x80, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0xDF, (byte)0xF0, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0xBB, (byte)0xFE, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x97, (byte)0x7F, (byte)0xC0, (byte)0x00, (byte)0x00, (byte)0x00, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x72, (byte)0xEF, (byte)0xF8, (byte)0x00, (byte)0x00, (byte)0x00, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x4E, (byte)0x5D, (byte)0xFF, (byte)0x00, (byte)0x00, (byte)0x00, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x29, (byte)0xCB, (byte)0xBF, (byte)0xE0, (byte)0x00, (byte)0x00, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x05, (byte)0x39, (byte)0x77, (byte)0xFC, (byte)0x00, (byte)0x00, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x00, (byte)0xA7, (byte)0x2E, (byte)0xFF, (byte)0x80, (byte)0x00, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x00, (byte)0x14, (byte)0xE5, (byte)0xDF, (byte)0xF0, (byte)0x00, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x00, (byte)0x02, (byte)0x9C, (byte)0xBB, (byte)0xFE, (byte)0x00, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x00, (byte)0x00, (byte)0x53, (byte)0x97, (byte)0x7F, (byte)0xC0, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x00, (byte)0x00, (byte)0x0A, (byte)0x72, (byte)0xEF, (byte)0xF8, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x00, (byte)0x00, (byte)0x01, (byte)0x4E, (byte)0x5D, (byte)0xFF, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x29, (byte)0xCB, (byte)0xBF, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x05, (byte)0x39, (byte)0x77, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0xA7, (byte)0x2E, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x14, (byte)0xE5, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x02, (byte)0x9C, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x53, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x0A, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x01, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x01, '\n'};
		test.evaluate(command);
		command = new byte[]{'D', (byte)0x00, (byte)0x0A, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, '\n'};
		test.evaluate(command);
		
		//Start the animation
		command = new byte[]{'T', 0x00, (byte)0x64, '\n'};
		test.evaluate(command);
		
		command = new byte[]{'S', '\n'};
		test.evaluate(command);
	}
}