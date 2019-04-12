
import com.sun.glass.events.KeyEvent;

import jssc.*;

public class SerialComm {

	SerialPort port;

	private boolean debug;  // Indicator of "debugging mode"
	
	// This function can be called to enable or disable "debugging mode"
	void setDebug(boolean mode) {
		debug = mode;
	}	
	

	// Constructor for the SerialComm class
	public SerialComm(String name) throws SerialPortException {
		port = new SerialPort(name);		
		port.openPort();
		port.setParams(SerialPort.BAUDRATE_9600,
			SerialPort.DATABITS_8,
			SerialPort.STOPBITS_1,
			SerialPort.PARITY_NONE);
		
		debug = false; // Default is to NOT be in debug mode
	}
		
	// TODO: Add writeByte() method 
	public void writeByte(byte b) { 
		 
		try {
			port.writeByte(b);
		} catch (SerialPortException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
}
	
	// TODO: Add available() method
	public boolean available () {
		try {
			if (port.getInputBufferBytesCount() != 0) {
				return true;	
			}
		} catch (SerialPortException e) {
			e.printStackTrace();
		}
		return false;
	}
	
	// TODO: Add readByte() method	
	public byte readByte () {
		try {
			byte b = port.readBytes(1)[0];
			if (debug) {
	
				System.out.println(String.format("%02x", b));
			}
			return b;
			
		} catch (SerialPortException e) {
			e.printStackTrace();
			return 0;
		}
	}
	
	// TODO: Add a main() method
	public static void main(String[]args) throws SerialPortException {
		try {
			SerialComm c = new SerialComm("/dev/cu.usbserial-DN03FDMX");	
//			DN0518PU
			byte store = 0;
			while (true) {
//				c.available();
				//System.out.println("" + c.readByte());
//				c.readByte();
				byte b = 0;
				if (StdDraw.isKeyPressed(KeyEvent.VK_UP)) {	
					b |= 8;
				}
				if (StdDraw.isKeyPressed(KeyEvent.VK_DOWN)) {
					b |= 4;
				}
				if (StdDraw.isKeyPressed(KeyEvent.VK_LEFT)) {
					b |= 2;
				}
				if (StdDraw.isKeyPressed(KeyEvent.VK_RIGHT)) {
					b |= 1;
				}
				if (store != b) {
					c.writeByte(b);
					System.out.println(b);
					store = b;
				}
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
