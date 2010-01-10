import junit.framework.*;
import java.net.Socket;
import java.net.InetAddress;
import java.io.OutputStream;
import java.io.IOException;

public class TestCmdProc extends TestCase {

	public void testAdd() {
		CmdProc proc;
		byte expected[] = new byte[]{(byte)0x7E, (byte)0x00, (byte)0x00, (byte)0x11};

		try {
			System.out.println("Creating CmdProc");
			proc = new CmdProc();
			System.out.println("Creating socket");
			Socket sock = new Socket(InetAddress.getByName(null), 35254);	
			System.out.println("Creating output stream");
			OutputStream stream = sock.getOutputStream();
			System.out.println("Writing data");
			stream.write(expected);
		} catch (IOException e) {
			fail();
			return;
		}

		System.out.println("Waiting for message");
		byte message[] = proc.waitForMessage();
		assertEquals(message, expected);
	}
}
